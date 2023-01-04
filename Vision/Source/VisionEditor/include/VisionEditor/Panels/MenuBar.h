#pragma once

#include <VisionUI/Panels/PanelMenuBar.h>
#include <VisionUI/Panels/PanelWindow.h>
#include <VisionUI/Widgets/Menu/MenuItem.h>

namespace VisionEditor::Panels
{
	class MenuBar : public VisionUI::Panels::PanelMenuBar
	{
		using PanelMap = std::unordered_map<std::string, std::pair<std::reference_wrapper<VisionUI::Panels::PanelWindow>, std::reference_wrapper<VisionUI::Widgets::Menu::MenuItem>>>;

	public:
		/**
		* Constructor
		*/
		MenuBar();
		
		/**
		* Check inputs for menubar shortcuts
		* @param p_deltaTime
		*/
		void HandleShortcuts(float p_deltaTime);

		/**
		* Register a panel to the menu bar window menu
		*/
		void RegisterPanel(const std::string& p_name, VisionUI::Panels::PanelWindow& p_panel);

	private:
		void CreateFileMenu();
		void CreateBuildMenu();
		void CreateWindowMenu();
		void CreateActorsMenu();
		void CreateResourcesMenu();
		void CreateSettingsMenu();
		void CreateLayoutMenu();
		void CreateHelpMenu();

		void UpdateToggleableItems();
		void OpenEveryWindows(bool p_state);

	private:
		PanelMap m_panels;

		VisionUI::Widgets::Menu::MenuList* m_windowMenu = nullptr;
	};
}