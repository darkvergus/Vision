#pragma once

#include <VisionUI/Widgets/Texts/Text.h>
#include <VisionUI/Panels/PanelWindow.h>
#include <VisionUI/Widgets/Layout/Group.h>
#include <VisionUI/Widgets/Layout/Columns.h>
#include <VisionRendering/Resources/Shader.h>

namespace VisionCore::Resources { class Material; }

namespace VisionEditor::Panels
{
	class MaterialEditor : public VisionUI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		MaterialEditor
		(
			const std::string& p_title,
			bool p_opened,
			const VisionUI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Refresh the material editor
		*/
		void Refresh();

		/**
		* Defines the target material of the material editor
		* @param p_newTarget
		*/
		void SetTarget(VisionCore::Resources::Material& p_newTarget);

		/**
		* Returns the target of the material editor
		*/
		VisionCore::Resources::Material* GetTarget() const;

		/**
		* Remove the target of the material editor (Clear the material editor)
		*/
		void RemoveTarget();

		/**
		* Launch the preview of the currently targeted material
		*/
		void Preview();

		/**
		* Reset material
		*/
		void Reset();
		
	private:
		void OnMaterialDropped();
		void OnShaderDropped();

		void CreateHeaderButtons();
		void CreateMaterialSelector();
		void CreateShaderSelector();
		void CreateMaterialSettings();
		void CreateShaderSettings();

		void GenerateShaderSettingsContent();
		void GenerateMaterialSettingsContent();

	private:
		VisionCore::Resources::Material* m_target		= nullptr;
		VisionRendering::Resources::Shader* m_shader	= nullptr;

		VisionUI::Widgets::Texts::Text* m_targetMaterialText	= nullptr;
		VisionUI::Widgets::Texts::Text* m_shaderText			= nullptr;

		VisionTools::Eventing::Event<> m_materialDroppedEvent;
		VisionTools::Eventing::Event<> m_shaderDroppedEvent;

		VisionUI::Widgets::Layout::Group* m_settings			= nullptr;
		VisionUI::Widgets::Layout::Group* m_materialSettings	= nullptr;
		VisionUI::Widgets::Layout::Group* m_shaderSettings		= nullptr;

		VisionUI::Widgets::Layout::Columns<2>* m_shaderSettingsColumns = nullptr;
		VisionUI::Widgets::Layout::Columns<2>* m_materialSettingsColumns = nullptr;
	};
}