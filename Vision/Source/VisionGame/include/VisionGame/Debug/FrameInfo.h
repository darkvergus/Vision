#pragma once

#ifdef _DEBUG

#include <VisionRendering/Core/Renderer.h>
#include <VisionWindowing/Window.h>

#include <VisionUI/Panels/PanelUndecorated.h>
#include <VisionUI/Widgets/Texts/TextColored.h>

namespace VisionGame::Debug
{
	/**
	* Panel that display information about the frame
	*/
	class FrameInfo : public VisionUI::Panels::PanelUndecorated
	{
	public:
		/**
		* Constructor
		* @param p_renderer
		* @param p_window
		*/
		FrameInfo(VisionRendering::Core::Renderer& p_renderer, VisionWindowing::Window& p_window);

		/**
		* Update the data
		* @parma p_deltaTime
		*/
		void Update(float p_deltaTime);

	private:
		VisionRendering::Core::Renderer&	m_renderer;
		VisionWindowing::Window&			m_window;

		VisionUI::Widgets::Texts::TextColored* m_frameInfo[3];
	};
}

#endif