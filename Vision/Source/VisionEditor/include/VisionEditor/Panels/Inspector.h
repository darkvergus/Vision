#pragma once

#include <VisionRendering/Resources/Loaders/TextureLoader.h>

#include <VisionUI/Panels/PanelWindow.h>
#include <VisionUI/Widgets/Layout/GroupCollapsable.h>
#include <VisionUI/Widgets/InputFields/InputText.h>
#include <VisionUI/Widgets/Visual/Separator.h>
#include <VisionUI/Widgets/Selection/CheckBox.h>
#include <VisionUI/Widgets/Buttons/Button.h>
#include <VisionUI/Widgets/Selection/ComboBox.h>

#include "VisionEditor/Panels/Hierarchy.h"
#include "VisionEditor/Panels/AssetBrowser.h"

namespace VisionEditor::Panels
{
	class Inspector : public VisionUI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		Inspector
		(
			const std::string& p_title,
			bool p_opened,
			const VisionUI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Destructor
		*/
		~Inspector();

		/**
		* Focus the given actor
		* @param p_target
		*/
		void FocusActor(VisionCore::ECS::Actor& p_target);

		/**
		* Unfocus the currently targeted actor
		*/
		void UnFocus();

		/**
		* Unfocus the currently targeted actor without removing listeners attached to this actor
		*/
		void SoftUnFocus();

		/**
		* Returns the currently selected actor
		*/
		VisionCore::ECS::Actor* GetTargetActor() const;

		/**
		* Create the actor inspector for the given actor
		*/
		void CreateActorInspector(VisionCore::ECS::Actor& p_target);

		/**
		* Draw the given component in inspector
		*/
		void DrawComponent(VisionCore::ECS::Components::AComponent& p_component);

		/**
		* Draw the given behaviour in inspector
		*/
		void DrawBehaviour(VisionCore::ECS::Components::Behaviour& p_behaviour);

		/**
		* Refresh the inspector
		*/
		void Refresh();

	private:
		VisionCore::ECS::Actor* m_targetActor = nullptr;
		VisionUI::Widgets::Layout::Group* m_actorInfo;
		VisionUI::Widgets::Layout::Group* m_inspectorHeader;
		VisionUI::Widgets::Selection::ComboBox* m_componentSelectorWidget;
        VisionUI::Widgets::InputFields::InputText* m_scriptSelectorWidget;

		uint64_t m_componentAddedListener	= 0;
		uint64_t m_componentRemovedListener = 0;
		uint64_t m_behaviourAddedListener	= 0;
		uint64_t m_behaviourRemovedListener = 0;
		uint64_t m_destroyedListener		= 0;
	};
}