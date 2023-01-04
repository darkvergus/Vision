#pragma once

#include <VisionRendering/LowRenderer/Camera.h>

#include <VisionCore/ECS/Actor.h>
#include <VisionCore/SceneSystem/SceneManager.h>
#include <VisionCore/ECS/Components/CModelRenderer.h>
#include <VisionCore/Resources/Material.h>
#include <VisionCore/ECS/Components/CAmbientBoxLight.h>
#include <VisionCore/ECS/Components/CAmbientSphereLight.h>

#include "VisionEditor/Core/Context.h"

namespace VisionEditor::Core { enum class EGizmoOperation; }
namespace VisionEditor::Panels { class AView; }

namespace VisionEditor::Core
{
	/**
	* Handle the rendering of the editor
	*/
	class EditorRenderer
	{
	public:
		/**
		* Create the EditorRenderer
		* @param p_context
		*/
		EditorRenderer(Context& p_context);

		/**
		* Initialize custom materials
		*/
		void InitMaterials();

		/**
		* Prepare the picking material by send it the color corresponding to the given actor
		* @param p_actor
		* @param p_material
		*/
		void PreparePickingMaterial(VisionCore::ECS::Actor& p_actor, VisionCore::Resources::Material& p_material);

		/**
		* Calculate the model matrix for a camera attached to the given actor
		* @param p_actor
		*/
		VisionMaths::FMatrix4 CalculateCameraModelMatrix(VisionCore::ECS::Actor& p_actor);

		/**
		* Render the scene
		* @param p_cameraPosition
		* @param p_camera
		*/
		void RenderScene(const VisionMaths::FVector3& p_cameraPosition, const VisionRendering::LowRenderer::Camera& p_camera, const VisionRendering::Data::Frustum* p_customFrustum = nullptr);

		/**
		* Render the scene for actor picking (Unlit version of the scene with colors indicating actor IDs)
		*/
		void RenderSceneForActorPicking();

		/**
		* Render the User Interface
		*/
		void RenderUI();

		/**
		* Render every scene cameras
		*/
		void RenderCameras();

		/**
		* Render every scene lights as billboards
		*/
		void RenderLights();

		/**
		* Render a gizmo at position
		* @param p_position
		* @param p_rotation
		* @param p_operation
		* @param p_pickable (Determine the shader to use to render the gizmo)
		* @param p_highlightedAxis (-1 to highlight no axis, 0 for X, 1 for Y, 2 for Z)
		*/
		void RenderGizmo(const VisionMaths::FVector3& p_position, const VisionMaths::FQuaternion& p_rotation, VisionEditor::Core::EGizmoOperation p_operation, bool p_pickable, int p_highlightedAxis = -1);

		/**
		* Render a model to the stencil buffer
		*/
		void RenderModelToStencil(const VisionMaths::FMatrix4& p_worldMatrix, VisionRendering::Resources::Model& p_model);

		/**
		* Render a model outline using the data stored in the stencil buffer
		* @param p_worldMatrix
		* @param p_model
		* @param p_width
		*/
		void RenderModelOutline(const VisionMaths::FMatrix4& p_worldMatrix, VisionRendering::Resources::Model& p_model, float p_width);

		/**
		* Render the actor as a selected actor (Outline)
		* @param p_actor
		* @param p_toStencil
		* @param p_isSelected
		*/
		void RenderActorOutlinePass(VisionCore::ECS::Actor& p_actor, bool p_toStencil, bool p_isSelected = false);

        /**
        * Render the camera perspective frustum
        * @param p_size
        * @param p_camera
        */
        void RenderCameraPerspectiveFrustum(std::pair<uint16_t, uint16_t>& p_size, VisionCore::ECS::Components::CCamera& p_camera);

        /**
        * Render the camera orthographic frustum
        * @param p_size
        * @param p_camera
        */
        void RenderCameraOrthographicFrustum(std::pair<uint16_t, uint16_t>& p_size, VisionCore::ECS::Components::CCamera& p_camera);

		/**
		* Render the camera frustum
		*/
		void RenderCameraFrustum(VisionCore::ECS::Components::CCamera& p_camera);

		/**
		* Render an actor collider
		*/
		void RenderActorCollider(VisionCore::ECS::Actor& p_actor);

		/**
		* Render light bounds
		*/
		void RenderLightBounds(VisionCore::ECS::Components::CLight& p_light);

		/**
		* Render ambient box volume
		*/
		void RenderAmbientBoxVolume(VisionCore::ECS::Components::CAmbientBoxLight& p_ambientBoxLight);

		/**
		* Render ambient sphere volume
		*/
		void RenderAmbientSphereVolume(VisionCore::ECS::Components::CAmbientSphereLight& p_ambientSphereLight);

		/**
		* Render the the bounding spheres of the given model renderer
		*/
		void RenderBoundingSpheres(VisionCore::ECS::Components::CModelRenderer& p_modelRenderer);

		/**
		* Render model
		*/
		void RenderModelAsset(VisionRendering::Resources::Model& p_model);

		/**
		* Render texture
		*/
		void RenderTextureAsset(VisionRendering::Resources::Texture& p_texture);

		/**
		* Render material
		*/
		void RenderMaterialAsset(VisionCore::Resources::Material& p_material);

		/**
		* Render the grid
		*/
		void RenderGrid(const VisionMaths::FVector3& p_viewPos, const VisionMaths::FVector3& p_color);

		/**
		* Update the light SSBO with the current scene
		* @param p_scene
		*/
		void UpdateLights(VisionCore::SceneSystem::Scene& p_scene);

		/**
		* Update the light SSBO with the current scene (Lights outside of the given frustum are culled)
		* @param p_scene
		*/
		void UpdateLightsInFrustum(VisionCore::SceneSystem::Scene& p_scene, const VisionRendering::Data::Frustum& p_frustum);

	private:
		Context& m_context;

		VisionCore::Resources::Material m_gridMaterial;
		VisionCore::Resources::Material m_stencilFillMaterial;
		VisionCore::Resources::Material m_textureMaterial;
		VisionCore::Resources::Material m_outlineMaterial;
		VisionCore::Resources::Material m_emptyMaterial;
		VisionCore::Resources::Material m_defaultMaterial;
		VisionCore::Resources::Material m_cameraMaterial;
		VisionCore::Resources::Material m_lightMaterial;
		VisionCore::Resources::Material m_gizmoArrowMaterial;
		VisionCore::Resources::Material m_gizmoBallMaterial;
		VisionCore::Resources::Material m_gizmoPickingMaterial;
		VisionCore::Resources::Material m_actorPickingMaterial;
	};
}