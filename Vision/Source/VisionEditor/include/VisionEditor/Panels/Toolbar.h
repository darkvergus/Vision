#pragma once

#include <VisionUI/Widgets/Buttons/ButtonImage.h>
#include <VisionUI/Panels/PanelWindow.h>

namespace VisionEditor::Panels
{
	class Toolbar : public VisionUI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		Toolbar
		(
			const std::string& p_title,
			bool p_opened,
			const VisionUI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Custom implementation of the draw method
		*/
		void _Draw_Impl() override;

	private:
		VisionUI::Widgets::Buttons::ButtonImage* m_playButton;
		VisionUI::Widgets::Buttons::ButtonImage* m_pauseButton;
		VisionUI::Widgets::Buttons::ButtonImage* m_stopButton;
		VisionUI::Widgets::Buttons::ButtonImage* m_nextButton;
	};
}