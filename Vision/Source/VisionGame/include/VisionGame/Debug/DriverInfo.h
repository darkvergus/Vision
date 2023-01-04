#pragma once

#ifdef _DEBUG

#include <VisionRendering/Core/Renderer.h>
#include <VisionWindowing/Window.h>

#include <VisionUI/Panels/PanelUndecorated.h>
#include <VisionUI/Widgets/Texts/TextColored.h>

namespace VisionGame::Debug
{
	/**
	* Panel that display information about the driver (hardware and software)
	*/
	class DriverInfo : public VisionUI::Panels::PanelUndecorated
	{
	public:
		/**
		* Constructor
		* @param p_renderer
		* @param p_window
		*/
		DriverInfo(VisionRendering::Core::Renderer& p_renderer, VisionWindowing::Window& p_window);
	};
}

#endif