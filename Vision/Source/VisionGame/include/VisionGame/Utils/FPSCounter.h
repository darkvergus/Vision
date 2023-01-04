#pragma once

#include <VisionUI/Panels/PanelUndecorated.h>
#include <VisionUI/Widgets/Texts/TextColored.h>

#include <VisionWindowing/Window.h>

namespace VisionGame::Utils
{
	/**
	* Panel that display the framerate
	*/
	class FPSCounter : public VisionUI::Panels::PanelUndecorated
	{
	public:
		/**
		* Constructor
		* @param p_window
		*/
		FPSCounter(VisionWindowing::Window& p_window);

		/**
		* Update the data
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);

	private:
		VisionUI::Widgets::Texts::TextColored m_text;

		VisionWindowing::Window& m_window;
		float m_elapsed = 0.0f;
		uint32_t m_frames = 0;
	};
}