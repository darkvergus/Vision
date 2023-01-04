#pragma once

#include <VisionRendering/Resources/Loaders/TextureLoader.h>
#include <VisionRendering/LowRenderer/Camera.h>

#include <VisionCore/SceneSystem/SceneManager.h>

#include <VisionUI/Panels/PanelWindow.h>
#include <VisionUI/Widgets/Layout/Group.h>
#include <VisionUI/Widgets/Layout/TreeNode.h>

namespace VisionEditor::Panels
{
	class Hierarchy : public VisionUI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		Hierarchy
		(
			const std::string& p_title,
			bool p_opened,
			const VisionUI::Settings::PanelWindowSettings& p_windowSettings
		);
		
		/**
		* Clear hierarchy nodes
		*/
		void Clear();

		/**
		* Unselect every widgets
		*/
		void UnselectActorsWidgets();

		/**
		* Select the widget corresponding to the given actor
		* @param p_actor
		*/
		void SelectActorByInstance(VisionCore::ECS::Actor& p_actor);

		/**
		* Select the widget
		* @param p_actor
		*/
		void SelectActorByWidget(VisionUI::Widgets::Layout::TreeNode& p_widget);

		/**
		* Attach the given actor linked widget to its parent widget
		* @param p_actor
		*/
		void AttachActorToParent(VisionCore::ECS::Actor& p_actor);

		/**
		* Detach the given actor linked widget from its parent widget
		* @param p_actor
		*/
		void DetachFromParent(VisionCore::ECS::Actor& p_actor);

		/**
		* Delete the widget referencing the given actor
		* @param p_actor
		*/
		void DeleteActorByInstance(VisionCore::ECS::Actor& p_actor);

		/**
		* Add a widget referencing the given actor
		* @param p_actor
		*/
		void AddActorByInstance(VisionCore::ECS::Actor& p_actor);

	public:
		VisionTools::Eventing::Event<VisionCore::ECS::Actor&> ActorSelectedEvent;
		VisionTools::Eventing::Event<VisionCore::ECS::Actor&> ActorUnselectedEvent;

	private:
		VisionUI::Widgets::Layout::Group* m_HierarchyHeader;
		VisionUI::Widgets::Layout::TreeNode* m_sceneRoot;

		std::unordered_map<VisionCore::ECS::Actor*, VisionUI::Widgets::Layout::TreeNode*> m_widgetActorLink;
	};
}