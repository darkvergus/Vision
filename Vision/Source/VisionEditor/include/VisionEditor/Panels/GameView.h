#pragma once

#include <VisionCore/SceneSystem/SceneManager.h>

#include "VisionEditor/Panels/AView.h"

namespace VisionEditor::Core { class EditorRenderer; }

namespace VisionEditor::Panels
{
	class GameView : public VisionEditor::Panels::AView
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		GameView
		(
			const std::string& p_title,
			bool p_opened,
			const VisionUI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Update the view
		* @param p_deltaTime
		*/
		virtual void Update(float p_deltaTime) override;

		/**
		* Custom implementation of the render method
		*/
		virtual void _Render_Impl() override;

		/**
		* Returns true if the game view has a camera
		*/
		bool HasCamera() const;

		/**
		* Returns the game view camera frustum or nothing if the game isn't playing
		*/
		std::optional<VisionRendering::Data::Frustum> GetActiveFrustum() const;

	private:
		VisionCore::SceneSystem::SceneManager& m_sceneManager;
		bool m_hasCamera = false;
	};
}