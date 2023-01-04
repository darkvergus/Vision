#pragma once

#include <VisionRendering/Core/Renderer.h>
#include <VisionWindowing/Context/Device.h>
#include <VisionWindowing/Window.h>
#include <VisionRendering/Context/Driver.h>
#include <VisionUI/Core/UIManager.h>
#include <VisionUI/Panels/PanelWindow.h>

namespace VisionEditor::Core
{
	/**
	* A simple panel that allow the user to select the project to launch
	*/
	class ProjectHub
	{
	public:
		/**
		* Constructor
		*/
		ProjectHub();

		/**
		* Run the project hub logic
		*/
		std::tuple<bool, std::string, std::string> Run();

		/**
		* Setup the project hub specific context (minimalist context)
		*/
		void SetupContext();

		/**
		* Register the project (identified from the given path) into the project hub
		* @param p_path
		*/
		void RegisterProject(const std::string& p_path);

	private:
		std::unique_ptr<VisionWindowing::Context::Device>		m_device;
		std::unique_ptr<VisionWindowing::Window>				m_window;
		std::unique_ptr<VisionRendering::Context::Driver>		m_driver;
		std::unique_ptr<VisionRendering::Core::Renderer>		m_renderer;
		std::unique_ptr<VisionUI::Core::UIManager>				m_uiManager;

		VisionUI::Modules::Canvas m_canvas;
		std::unique_ptr<VisionUI::Panels::PanelWindow>			m_mainPanel;

		std::string m_projectPath = "";
		std::string m_projectName = "";
		bool m_readyToGo = false;
	};
}