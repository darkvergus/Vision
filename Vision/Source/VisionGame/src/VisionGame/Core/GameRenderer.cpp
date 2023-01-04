#include "VisionGame/Core/GameRenderer.h"

#include <VisionAnalytics/Profiling/ProfilerSpy.h>

#include <VisionCore/ECS/Components/CMaterialRenderer.h>
#include <VisionCore/ECS/Components/CModelRenderer.h>
#include <VisionCore/ECS/Components/CPointLight.h>
#include <VisionCore/ECS/Components/CDirectionalLight.h>
#include <VisionCore/ECS/Components/CSpotLight.h>
#include <VisionCore/ECS/Components/CAmbientBoxLight.h>
#include <VisionCore/ECS/Components/CAmbientSphereLight.h>

using namespace VisionMaths;
using namespace VisionRendering::Resources;
using namespace VisionCore::Resources;

VisionGame::Core::GameRenderer::GameRenderer(Context & p_context) :
	m_context(p_context)
{
	/* Empty Material */
	m_emptyMaterial.SetShader(m_context.shaderManager[":Shaders\\Unlit.glsl"]);
	m_emptyMaterial.Set("u_Diffuse", FVector3(1.f, 0.f, 1.f));
	m_emptyMaterial.Set<VisionRendering::Resources::Texture*>("u_DiffuseMap", nullptr);

	m_context.renderer->RegisterModelMatrixSender([this](const VisionMaths::FMatrix4 & p_modelMatrix)
	{
		m_context.engineUBO->SetSubData(VisionMaths::FMatrix4::Transpose(p_modelMatrix), 0);
	});

	m_context.renderer->RegisterUserMatrixSender([this](const VisionMaths::FMatrix4 & p_userMatrix)
	{
		m_context.engineUBO->SetSubData
		(
			p_userMatrix,

			// UBO layout offset
			sizeof(VisionMaths::FMatrix4) +
			sizeof(VisionMaths::FMatrix4) +
			sizeof(VisionMaths::FMatrix4) +
			sizeof(VisionMaths::FVector3) +
			sizeof(float)
		);
	});
}

void VisionGame::Core::GameRenderer::RenderScene()
{
	if (auto currentScene = m_context.sceneManager.GetCurrentScene())
	{
		if (VisionCore::ECS::Components::CCamera* mainCameraComponent = m_context.renderer->FindMainCamera(*currentScene))
		{
			if (mainCameraComponent->HasFrustumLightCulling())
			{
				UpdateLightsInFrustum(*currentScene, mainCameraComponent->GetCamera().GetFrustum());
			}
			else
			{
				UpdateLights(*currentScene);
			}

			auto [winWidth, winHeight] = m_context.window->GetSize();
			const auto& cameraPosition = mainCameraComponent->owner.transform.GetWorldPosition();
			const auto& cameraRotation = mainCameraComponent->owner.transform.GetWorldRotation();
			auto& camera = mainCameraComponent->GetCamera();

			camera.CacheMatrices(winWidth, winHeight, cameraPosition, cameraRotation);

			UpdateEngineUBO(*mainCameraComponent);

			m_context.renderer->Clear(camera, true, true, false);

			uint8_t glState = m_context.renderer->FetchGLState();
			m_context.renderer->ApplyStateMask(glState);
			m_context.renderer->RenderScene(*currentScene, cameraPosition, camera, nullptr, &m_emptyMaterial);
			m_context.renderer->ApplyStateMask(glState);
		}
		else
		{
			m_context.renderer->SetClearColor(0.0f, 0.0f, 0.0f);
			m_context.renderer->Clear(true, true, false);
		}
	}
}

void VisionGame::Core::GameRenderer::UpdateEngineUBO(VisionCore::ECS::Components::CCamera& p_mainCamera)
{
	size_t offset = sizeof(VisionMaths::FMatrix4); // We skip the model matrix (Which is a special case, modified every draw calls)
	auto& camera = p_mainCamera.GetCamera();

	m_context.engineUBO->SetSubData(VisionMaths::FMatrix4::Transpose(camera.GetViewMatrix()), std::ref(offset));
	m_context.engineUBO->SetSubData(VisionMaths::FMatrix4::Transpose(camera.GetProjectionMatrix()), std::ref(offset));
	m_context.engineUBO->SetSubData(p_mainCamera.owner.transform.GetWorldPosition(), std::ref(offset));
}

void VisionGame::Core::GameRenderer::UpdateLights(VisionCore::SceneSystem::Scene& p_scene)
{
	PROFILER_SPY("Light SSBO Update");
	auto lightMatrices = m_context.renderer->FindLightMatrices(p_scene);
	m_context.lightSSBO->SendBlocks<FMatrix4>(lightMatrices.data(), lightMatrices.size() * sizeof(FMatrix4));
}

void VisionGame::Core::GameRenderer::UpdateLightsInFrustum(VisionCore::SceneSystem::Scene& p_scene, const VisionRendering::Data::Frustum& p_frustum)
{
	PROFILER_SPY("Light SSBO Update (Frustum culled)");
	auto lightMatrices = m_context.renderer->FindLightMatricesInFrustum(p_scene, p_frustum);
	m_context.lightSSBO->SendBlocks<FMatrix4>(lightMatrices.data(), lightMatrices.size() * sizeof(FMatrix4));
}
