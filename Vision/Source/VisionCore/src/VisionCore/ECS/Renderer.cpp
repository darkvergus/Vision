#include <VisionAnalytics/Profiling/ProfilerSpy.h>

#include <VisionRendering/Resources/Loaders/TextureLoader.h>
#include <VisionRendering/Data/Frustum.h>

#include "VisionCore/ECS/Renderer.h"
#include "VisionCore/ECS/Components/CModelRenderer.h"
#include "VisionCore/ECS/Components/CMaterialRenderer.h"

VisionCore::ECS::Renderer::Renderer(VisionRendering::Context::Driver& p_driver) :
	VisionRendering::Core::Renderer(p_driver),
	m_emptyTexture(VisionRendering::Resources::Loaders::TextureLoader::CreateColor
	(
		(255 << 24) | (255 << 16) | (255 << 8) | 255,
		VisionRendering::Settings::ETextureFilteringMode::NEAREST,
		VisionRendering::Settings::ETextureFilteringMode::NEAREST,
		false
	))
{
}

VisionCore::ECS::Renderer::~Renderer()
{
	VisionRendering::Resources::Loaders::TextureLoader::Destroy(m_emptyTexture);
}

VisionCore::ECS::Components::CCamera* VisionCore::ECS::Renderer::FindMainCamera(const VisionCore::SceneSystem::Scene& p_scene)
{
	for (VisionCore::ECS::Components::CCamera* camera : p_scene.GetFastAccessComponents().cameras)
		if (camera->owner.IsActive())
			return camera;

	return nullptr;
}

std::vector<VisionMaths::FMatrix4> VisionCore::ECS::Renderer::FindLightMatrices(const VisionCore::SceneSystem::Scene& p_scene)
{
	std::vector<VisionMaths::FMatrix4> result;

	const auto& facs = p_scene.GetFastAccessComponents();

	for (auto light : facs.lights)
	{
		if (light->owner.IsActive())
		{
			result.push_back(light->GetData().GenerateMatrix());
		}
	}

	return result;
}



std::vector<VisionMaths::FMatrix4> VisionCore::ECS::Renderer::FindLightMatricesInFrustum(const VisionCore::SceneSystem::Scene& p_scene, const VisionRendering::Data::Frustum& p_frustum)
{
	std::vector<VisionMaths::FMatrix4> result;

	const auto& facs = p_scene.GetFastAccessComponents();

	for (auto light : facs.lights)
	{
		if (light->owner.IsActive())
		{
			const auto& lightData = light->GetData();
			const auto& position = lightData.GetTransform().GetWorldPosition();
			auto effectRange = lightData.GetEffectRange();

			// We always consider lights that have an +inf range (Not necessary to test if they are in frustum)
			if (std::isinf(effectRange) || p_frustum.SphereInFrustum(position.x, position.y, position.z, lightData.GetEffectRange()))
			{
				result.push_back(lightData.GenerateMatrix());
			}
		}
	}

	return result;
}

void VisionCore::ECS::Renderer::RenderScene
(
	VisionCore::SceneSystem::Scene& p_scene,
	const VisionMaths::FVector3& p_cameraPosition,
	const VisionRendering::LowRenderer::Camera& p_camera,
	const VisionRendering::Data::Frustum* p_customFrustum,
	VisionCore::Resources::Material* p_defaultMaterial
)
{
	OpaqueDrawables	opaqueMeshes;
	TransparentDrawables transparentMeshes;

	if (p_camera.HasFrustumGeometryCulling())
	{
		const auto& frustum = p_customFrustum ? *p_customFrustum : p_camera.GetFrustum();
		std::tie(opaqueMeshes, transparentMeshes) = FindAndSortFrustumCulledDrawables(p_scene, p_cameraPosition, frustum, p_defaultMaterial);
	}
	else
	{
		std::tie(opaqueMeshes, transparentMeshes) = FindAndSortDrawables(p_scene, p_cameraPosition, p_defaultMaterial);
	}

	for (const auto& [distance, drawable] : opaqueMeshes)
		DrawDrawable(drawable);

	for (const auto& [distance, drawable] : transparentMeshes)
		DrawDrawable(drawable);
}

void FindAndSortDrawables
(
	VisionCore::ECS::Renderer::OpaqueDrawables& p_opaques,
	VisionCore::ECS::Renderer::TransparentDrawables& p_transparents,
	const VisionCore::SceneSystem::Scene& p_scene,
	const VisionMaths::FVector3& p_cameraPosition,
	VisionCore::Resources::Material* p_defaultMaterial
)
{
	for (VisionCore::ECS::Components::CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		if (modelRenderer->owner.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				float distanceToActor = VisionMaths::FVector3::Distance(modelRenderer->owner.transform.GetWorldPosition(), p_cameraPosition);

				if (auto materialRenderer = modelRenderer->owner.GetComponent<VisionCore::ECS::Components::CMaterialRenderer>())
				{
					const auto& transform = modelRenderer->owner.transform.GetFTransform();

					const VisionCore::ECS::Components::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

					for (auto mesh : model->GetMeshes())
					{
						VisionCore::Resources::Material* material = nullptr;

						if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
						{
							material = materials.at(mesh->GetMaterialIndex());
							if (!material || !material->GetShader())
								material = p_defaultMaterial;
						}

						if (material)
						{
							VisionCore::ECS::Renderer::Drawable element = { transform.GetWorldMatrix(), mesh, material, materialRenderer->GetUserMatrix() };

							if (material->IsBlendable())
								p_transparents.emplace(distanceToActor, element);
							else
								p_opaques.emplace(distanceToActor, element);
						}
					}
				}
			}
		}
	}
}

std::pair<VisionCore::ECS::Renderer::OpaqueDrawables, VisionCore::ECS::Renderer::TransparentDrawables> VisionCore::ECS::Renderer::FindAndSortFrustumCulledDrawables
(
	const VisionCore::SceneSystem::Scene& p_scene,
	const VisionMaths::FVector3& p_cameraPosition,
	const VisionRendering::Data::Frustum& p_frustum,
	VisionCore::Resources::Material* p_defaultMaterial
)
{
	using namespace VisionCore::ECS::Components;

	VisionCore::ECS::Renderer::OpaqueDrawables opaqueDrawables;
	VisionCore::ECS::Renderer::TransparentDrawables transparentDrawables;

	for (CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		auto& owner = modelRenderer->owner;

		if (owner.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				if (auto materialRenderer = modelRenderer->owner.GetComponent<CMaterialRenderer>())
				{
					auto& transform = owner.transform.GetFTransform();

					VisionRendering::Settings::ECullingOptions cullingOptions = VisionRendering::Settings::ECullingOptions::NONE;

					if (modelRenderer->GetFrustumBehaviour() != CModelRenderer::EFrustumBehaviour::DISABLED)
					{
						cullingOptions |= VisionRendering::Settings::ECullingOptions::FRUSTUM_PER_MODEL;
					}

					if (modelRenderer->GetFrustumBehaviour() == CModelRenderer::EFrustumBehaviour::CULL_MESHES)
					{
						cullingOptions |= VisionRendering::Settings::ECullingOptions::FRUSTUM_PER_MESH;
					}

					const auto& modelBoundingSphere = modelRenderer->GetFrustumBehaviour() == CModelRenderer::EFrustumBehaviour::CULL_CUSTOM ? modelRenderer->GetCustomBoundingSphere() : model->GetBoundingSphere();

					std::vector<std::reference_wrapper<VisionRendering::Resources::Mesh>> meshes;

					{
						PROFILER_SPY("Frustum Culling");
						meshes = GetMeshesInFrustum(*model, modelBoundingSphere, transform, p_frustum, cullingOptions);
					}

					if (!meshes.empty())
					{
						float distanceToActor = VisionMaths::FVector3::Distance(transform.GetWorldPosition(), p_cameraPosition);
						const VisionCore::ECS::Components::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

						for (const auto& mesh : meshes)
						{
							VisionCore::Resources::Material* material = nullptr;

							if (mesh.get().GetMaterialIndex() < MAX_MATERIAL_COUNT)
							{
								material = materials.at(mesh.get().GetMaterialIndex());
								if (!material || !material->GetShader())
									material = p_defaultMaterial;
							}

							if (material)
							{
								VisionCore::ECS::Renderer::Drawable element = { transform.GetWorldMatrix(), &mesh.get(), material, materialRenderer->GetUserMatrix() };

								if (material->IsBlendable())
									transparentDrawables.emplace(distanceToActor, element);
								else
									opaqueDrawables.emplace(distanceToActor, element);
							}
						}
					}
				}
			}
		}
	}

	return { opaqueDrawables, transparentDrawables };
}

std::pair<VisionCore::ECS::Renderer::OpaqueDrawables, VisionCore::ECS::Renderer::TransparentDrawables> VisionCore::ECS::Renderer::FindAndSortDrawables
(
	const VisionCore::SceneSystem::Scene& p_scene,
	const VisionMaths::FVector3& p_cameraPosition,
	VisionCore::Resources::Material* p_defaultMaterial
)
{
	VisionCore::ECS::Renderer::OpaqueDrawables opaqueDrawables;
	VisionCore::ECS::Renderer::TransparentDrawables transparentDrawables;

	for (VisionCore::ECS::Components::CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		if (modelRenderer->owner.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				float distanceToActor = VisionMaths::FVector3::Distance(modelRenderer->owner.transform.GetWorldPosition(), p_cameraPosition);

				if (auto materialRenderer = modelRenderer->owner.GetComponent<VisionCore::ECS::Components::CMaterialRenderer>())
				{
					const auto& transform = modelRenderer->owner.transform.GetFTransform();

					const VisionCore::ECS::Components::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

					for (auto mesh : model->GetMeshes())
					{
						VisionCore::Resources::Material* material = nullptr;

						if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
						{
							material = materials.at(mesh->GetMaterialIndex());
							if (!material || !material->GetShader())
								material = p_defaultMaterial;
						}

						if (material)
						{
							VisionCore::ECS::Renderer::Drawable element = { transform.GetWorldMatrix(), mesh, material, materialRenderer->GetUserMatrix() };

							if (material->IsBlendable())
								transparentDrawables.emplace(distanceToActor, element);
							else
								opaqueDrawables.emplace(distanceToActor, element);
						}
					}
				}
			}
		}
	}

	return { opaqueDrawables, transparentDrawables };
}

void VisionCore::ECS::Renderer::DrawDrawable(const Drawable& p_toDraw)
{
	m_userMatrixSender(std::get<3>(p_toDraw));
	DrawMesh(*std::get<1>(p_toDraw), *std::get<2>(p_toDraw), &std::get<0>(p_toDraw));
}

void VisionCore::ECS::Renderer::DrawModelWithSingleMaterial(VisionRendering::Resources::Model& p_model, VisionCore::Resources::Material& p_material, VisionMaths::FMatrix4 const* p_modelMatrix, VisionCore::Resources::Material* p_defaultMaterial)
{
	if (p_modelMatrix)
		m_modelMatrixSender(*p_modelMatrix);

	for (auto mesh : p_model.GetMeshes())
	{
		VisionCore::Resources::Material* material = p_material.GetShader() ? &p_material : p_defaultMaterial;

		if (material)
			DrawMesh(*mesh, *material, nullptr);
	}
}

void VisionCore::ECS::Renderer::DrawModelWithMaterials(VisionRendering::Resources::Model& p_model, std::vector<VisionCore::Resources::Material*> p_materials, VisionMaths::FMatrix4 const* p_modelMatrix, VisionCore::Resources::Material* p_defaultMaterial)
{
	if (p_modelMatrix)
		m_modelMatrixSender(*p_modelMatrix);

	for (auto mesh : p_model.GetMeshes())
	{
		VisionCore::Resources::Material* material = p_materials.size() > mesh->GetMaterialIndex() ? p_materials[mesh->GetMaterialIndex()] : p_defaultMaterial;
		if (material)
			DrawMesh(*mesh, *material, nullptr);
	}
}

void VisionCore::ECS::Renderer::DrawMesh(VisionRendering::Resources::Mesh& p_mesh, VisionCore::Resources::Material& p_material, VisionMaths::FMatrix4 const* p_modelMatrix)
{
	using namespace VisionRendering::Settings;

	if (p_material.HasShader() && p_material.GetGPUInstances() > 0)
	{
		if (p_modelMatrix)
			m_modelMatrixSender(*p_modelMatrix);

		uint8_t stateMask = p_material.GenerateStateMask();
		ApplyStateMask(stateMask);
		
		/* Draw the mesh */
		p_material.Bind(m_emptyTexture);
		Draw(p_mesh, VisionRendering::Settings::EPrimitiveMode::TRIANGLES, p_material.GetGPUInstances());
		p_material.UnBind();
	}
}

void VisionCore::ECS::Renderer::RegisterModelMatrixSender(std::function<void(VisionMaths::FMatrix4)> p_modelMatrixSender)
{
	m_modelMatrixSender = p_modelMatrixSender;
}

void VisionCore::ECS::Renderer::RegisterUserMatrixSender(std::function<void(VisionMaths::FMatrix4)> p_userMatrixSender)
{
	m_userMatrixSender = p_userMatrixSender;
}
