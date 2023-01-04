#pragma once

#include <VisionCore/ECS/Actor.h>
#include <VisionCore/SceneSystem/SceneManager.h>
#include <VisionCore/ECS/Components/CCamera.h>

#include "VisionGame/Core/Context.h"

namespace VisionGame::Core
{
	/**
	* Handle the rendering of the game scene
	*/
	class GameRenderer
	{
	public:
		/**
		* Create the EditorRenderer
		* @param p_context
		*/
		GameRenderer(Context& p_context);

		/**
		* Render the scene
		*/
		void RenderScene();

		/**
		* Update the engine UBO
		* @param p_mainCamera
		*/
		void UpdateEngineUBO(VisionCore::ECS::Components::CCamera& p_mainCamera);

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
		VisionCore::Resources::Material m_emptyMaterial;
	};
}