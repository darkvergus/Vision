#pragma once

#include "VisionEditor/Panels/AViewControllable.h"
#include "VisionEditor/Core/GizmoBehaviour.h"

namespace VisionEditor::Panels
{
	class SceneView : public VisionEditor::Panels::AViewControllable
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		SceneView
		(
			const std::string& p_title,
			bool p_opened,
			const VisionUI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Update the scene view
		*/
		virtual void Update(float p_deltaTime) override;

		/**
		* Custom implementation of the render method
		*/
		virtual void _Render_Impl() override;

		/**
		* Render the actual scene
		* @param p_defaultRenderState
		*/
		void RenderScene(uint8_t p_defaultRenderState);

		/**
		* Render the scene for actor picking (Using unlit colors)
		*/
		void RenderSceneForActorPicking();

		/**
		* Render the scene for actor picking and handle the logic behind it
		*/
		void HandleActorPicking();

	private:
		VisionCore::SceneSystem::SceneManager& m_sceneManager;
		VisionRendering::Buffers::Framebuffer m_actorPickingFramebuffer;
		VisionEditor::Core::GizmoBehaviour m_gizmoOperations;
		VisionEditor::Core::EGizmoOperation m_currentOperation = VisionEditor::Core::EGizmoOperation::TRANSLATE;

		std::optional<std::reference_wrapper<VisionCore::ECS::Actor>> m_highlightedActor;
		std::optional<VisionEditor::Core::GizmoBehaviour::EDirection> m_highlightedGizmoDirection;
	};
}