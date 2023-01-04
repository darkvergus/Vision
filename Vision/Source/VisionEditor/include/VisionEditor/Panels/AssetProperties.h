#pragma once

#include <variant>

#include <VisionTools/Filesystem/IniFile.h>
#include <VisionTools/Eventing/Event.h>

#include <VisionUI/Widgets/Texts/Text.h>
#include <VisionUI/Panels/PanelWindow.h>
#include <VisionUI/Widgets/Layout/Group.h>
#include <VisionUI/Widgets/Layout/Columns.h>

#include <VisionRendering/Resources/Model.h>
#include <VisionRendering/Resources/Texture.h>

namespace VisionEditor::Panels
{
	class AssetProperties : public VisionUI::Panels::PanelWindow
	{
	public:
		using EditableAssets = std::variant<VisionRendering::Resources::Model*, VisionRendering::Resources::Texture*>;

		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		AssetProperties
		(
			const std::string& p_title,
			bool p_opened,
			const VisionUI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Defines the target of the asset settings editor
		* @param p_path
		*/
		void SetTarget(const std::string& p_path);

        /**
        * Refresh the panel to show the current target settings
        */
        void Refresh();

		/**
		* Launch the preview of the target asset
		*/
		void Preview();

	private:
		void CreateHeaderButtons();
        void CreateAssetSelector();
		void CreateSettings();
		void CreateInfo();
		void CreateModelSettings();
		void CreateTextureSettings();
		void Apply();

	private:
		std::string m_resource;

        VisionTools::Eventing::Event<> m_targetChanged;
        VisionUI::Widgets::Layout::Group* m_settings = nullptr;
        VisionUI::Widgets::Layout::Group* m_info = nullptr;
        VisionUI::Widgets::Buttons::Button* m_applyButton = nullptr;
        VisionUI::Widgets::Buttons::Button* m_revertButton = nullptr;
        VisionUI::Widgets::Buttons::Button* m_previewButton = nullptr;
        VisionUI::Widgets::Buttons::Button* m_resetButton = nullptr;
        VisionUI::Widgets::AWidget* m_headerSeparator = nullptr;
        VisionUI::Widgets::AWidget* m_headerLineBreak = nullptr;
		VisionUI::Widgets::Layout::Columns<2>* m_settingsColumns = nullptr;
		VisionUI::Widgets::Layout::Columns<2>* m_infoColumns = nullptr;
        VisionUI::Widgets::Texts::Text* m_assetSelector = nullptr;
		std::unique_ptr<VisionTools::Filesystem::IniFile> m_metadata;
	};
}