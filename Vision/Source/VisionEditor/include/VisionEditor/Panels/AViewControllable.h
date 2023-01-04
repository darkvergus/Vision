#pragma once

#include "VisionEditor/Panels/AView.h"
#include "VisionEditor/Core/CameraController.h"

namespace VisionEditor::Panels
{
	class AViewControllable : public VisionEditor::Panels::AView
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		* @param p_enableFocusInputs
		*/
		AViewControllable
		(
			const std::string& p_title,
			bool p_opened,
			const VisionUI::Settings::PanelWindowSettings& p_windowSettings,
			bool p_enableFocusInputs = false
		);

		/**
		* Update the controllable view (Handle inputs)
		* @param p_deltaTime
		*/
		virtual void Update(float p_deltaTime) override;

		/**
		* Returns the camera controller of the controllable view
		*/
		VisionEditor::Core::CameraController& GetCameraController();

	protected:
		VisionEditor::Core::CameraController m_cameraController;
	};
}