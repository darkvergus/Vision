#pragma once

#include <filesystem>
#include <unordered_map>
#include <queue>

#include <VisionUI/Panels/PanelWindow.h>
#include <VisionUI/Widgets/Layout/TreeNode.h>
#include <VisionRendering/Resources/Loaders/TextureLoader.h>
#include <VisionTools/Filesystem/FileWatcher.h>

namespace VisionEditor::Panels
{
	/**
	* A panel that handle asset management
	*/
	class AssetBrowser : public VisionUI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @parma p_opened
		* @param p_windowSettings
		* @param p_engineAssetFolder
		* @param p_projectAssetFolder
		* @param p_projectScriptFolder
		*/
		AssetBrowser
		(
			const std::string& p_title,
			bool p_opened,
			const VisionUI::Settings::PanelWindowSettings& p_windowSettings,
			const std::string& p_engineAssetFolder = "",
			const std::string& p_projectAssetFolder = "",
			const std::string& p_projectScriptFolder = ""
		);

		/**
		* Fill the asset browser panels with widgets corresponding to elements in the asset folder
		*/
		void Fill();

		/**
		* Clear the asset browser widgets
		*/
		void Clear();

		/**
		* Refresh the asset browser widgets (Clear + Fill)
		*/
		void Refresh();

	private:
		void ParseFolder(VisionUI::Widgets::Layout::TreeNode& p_root, const std::filesystem::directory_entry& p_directory, bool p_isEngineItem, bool p_scriptFolder = false);
		void ConsiderItem(VisionUI::Widgets::Layout::TreeNode* p_root, const std::filesystem::directory_entry& p_entry, bool p_isEngineItem, bool p_autoOpen = false, bool p_scriptFolder = false);

	public:
		static const std::string __FILENAMES_CHARS;

	private:
		std::string m_engineAssetFolder;
		std::string m_projectAssetFolder;
		std::string m_projectScriptFolder;
		VisionTools::Filesystem::FileWatcher m_projectAssetWatcher;
		VisionUI::Widgets::Layout::Group* m_assetList;
		std::unordered_map<VisionUI::Widgets::Layout::TreeNode*, std::string> m_pathUpdate;
	};
}