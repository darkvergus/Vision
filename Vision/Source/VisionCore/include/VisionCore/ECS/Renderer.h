#pragma once

#include <map>

#include <VisionRendering/Core/Renderer.h>
#include <VisionRendering/Resources/Mesh.h>
#include <VisionRendering/Data/Frustum.h>


#include "VisionCore/Resources/Material.h"
#include "VisionCore/ECS/Actor.h"
#include "VisionCore/ECS/Components/CCamera.h"
#include "VisionCore/SceneSystem/Scene.h"

namespace VisionCore::ECS
{
	/**
	* A Renderer capable of rendering stuffs linked with the ECS. It is a convenient class that should be used instead of VisionRendering::Core::Renderer
	* when you plan to use the OvCore ECS architecture.
	*/
	class Renderer : public VisionRendering::Core::Renderer
	{
	public:
		using Drawable				= std::tuple<VisionMaths::FMatrix4, VisionRendering::Resources::Mesh*, VisionCore::Resources::Material*, VisionMaths::FMatrix4>;
		using OpaqueDrawables		= std::multimap<float, Drawable, std::less<float>>;
		using TransparentDrawables	= std::multimap<float, Drawable, std::greater<float>>;

		/**
		* Constructor of the Renderer
		* @param p_driver
		*/
		Renderer(VisionRendering::Context::Driver& p_driver);

		/**
		* Destructor of the Renderer
		* @param p_driver
		*/
		~Renderer();

		/**
		* Parse a scene to find the main camera
		* @param p_scene
		*/
		VisionCore::ECS::Components::CCamera* FindMainCamera(const VisionCore::SceneSystem::Scene& p_scene);

		/**
		* Fill the given FMatrix4 vector with lights information
		* @param p_scene
		*/
		std::vector<VisionMaths::FMatrix4> FindLightMatrices(const VisionCore::SceneSystem::Scene& p_scene);

		/**
		* Fill the given FMatrix4 vector with lights information that are inside the frustum
		* @param p_scene
		* @param p_frustum
		*/
		std::vector<VisionMaths::FMatrix4> FindLightMatricesInFrustum(const VisionCore::SceneSystem::Scene& p_scene, const VisionRendering::Data::Frustum& p_frustum);

		/**
		* Draw the given scene using the given default material (optional) if no material found on an actor
		* @param p_scene
		* @param p_cameraPosition
		* @param p_camera
		* @param p_customFrustum
		* @param p_defaultMaterial
		*/
		void RenderScene
		(
			VisionCore::SceneSystem::Scene& p_scene,
			const VisionMaths::FVector3& p_cameraPosition,
			const VisionRendering::LowRenderer::Camera& p_camera,
			const VisionRendering::Data::Frustum* p_customFrustum = nullptr,
			VisionCore::Resources::Material* p_defaultMaterial = nullptr
		);

		/**
		* Returns opaque and transparents drawables from the scene with frustum culling
		* @param p_scene
		* @param p_cameraPosition
		* @param p_frustum
		* @param p_defaultMaterial
		*/
		std::pair<OpaqueDrawables, TransparentDrawables> FindAndSortFrustumCulledDrawables
		(
			const VisionCore::SceneSystem::Scene& p_scene,
			const VisionMaths::FVector3& p_cameraPosition,
			const VisionRendering::Data::Frustum& p_frustum,
			VisionCore::Resources::Material* p_defaultMaterial
		);

		/**
		* Returns opaque and transparents drawables from the scene
		* @param p_scene
		* @param p_cameraPosition
		* @param p_defaultMaterial
		*/
		std::pair<OpaqueDrawables, TransparentDrawables> FindAndSortDrawables
		(
			const VisionCore::SceneSystem::Scene& p_scene,
			const VisionMaths::FVector3& p_cameraPosition,
			VisionCore::Resources::Material* p_defaultMaterial
		);

		/**
		* Draw a Drawable instance
		* @param p_toDraw
		*/
		void DrawDrawable(const Drawable& p_toDraw);

		/**
		* Draw the model using the given material (The material will be applied to every submeshes of the the model)
		* @param p_model
		* @param p_material
		* @param p_modelMatrix
		* @param p_defaultMaterial (Used if the given material has no shader attached)
		*/
		void DrawModelWithSingleMaterial(VisionRendering::Resources::Model& p_model, VisionCore::Resources::Material& p_material, VisionMaths::FMatrix4 const* p_modelMatrix, VisionCore::Resources::Material* p_defaultMaterial = nullptr);

		/**
		* Draw the model using the given materials
		* @param p_model
		* @param p_modelMatrix
		* @param p_defaultMaterial (Used when a submesh material has no shader attached)
		*/
		void DrawModelWithMaterials(VisionRendering::Resources::Model& p_model, std::vector<VisionCore::Resources::Material*> p_materials, VisionMaths::FMatrix4 const* p_modelMatrix, VisionCore::Resources::Material* p_defaultMaterial = nullptr);

		/**
		* Try drawing a mesh using the given material (Fails if the material has no shader attached)
		* @param p_mesh
		* @param p_material
		* @param p_modelMatrix (If set to nullptr, no data will be sent to the GPU)
		*/
		void DrawMesh(VisionRendering::Resources::Mesh& p_mesh, VisionCore::Resources::Material& p_material, VisionMaths::FMatrix4 const* p_modelMatrix);

		/**
		* Register the given function as the model matrix sender.
		* The given function will be called before every draw calls.
		* You should use this function to send the model matrix
		* to the GPU in the way you want
		* @param p_modelMatrixSender
		*/
		void RegisterModelMatrixSender(std::function<void(VisionMaths::FMatrix4)> p_modelMatrixSender);

		/**
		* Register the given function as the user matrix sender.
		* The given function will be called before every draw calls.
		* You should use this function to send the user matrix
		* to the GPU in the way you want
		* @param p_userMatrixSender
		*/
		void RegisterUserMatrixSender(std::function<void(VisionMaths::FMatrix4)> p_userMatrixSender);

	private:
		std::function<void(VisionMaths::FMatrix4)> m_modelMatrixSender;
		std::function<void(VisionMaths::FMatrix4)> m_userMatrixSender;
		VisionRendering::Resources::Texture* m_emptyTexture = nullptr;
	};
}