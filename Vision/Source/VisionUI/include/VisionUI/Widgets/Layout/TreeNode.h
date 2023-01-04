#pragma once

#include <vector>

#include <VisionTools/Eventing/Event.h>

#include "VisionUI/Internal/WidgetContainer.h"
#include "VisionUI/Widgets/DataWidget.h"

namespace VisionUI::Widgets::Layout
{
	/**
	* Widget that allow columnification
	*/
	class TreeNode : public DataWidget<std::string>, public Internal::WidgetContainer 
	{
	public:
		/**
		* Constructor
		* @param p_name
		* @param p_arrowClickToOpen
		*/
		TreeNode(const std::string& p_name = "", bool p_arrowClickToOpen = false);

		/**
		* Open the tree node
		*/
		void Open();

		/**
		* Close the tree node
		*/
		void Close();

		/**
		* Returns true if the TreeNode is currently opened
		*/
		bool IsOpened() const;

	protected:
		virtual void _Draw_Impl() override;

	public:
		std::string name;
		bool selected = false;
		bool leaf = false;

		VisionTools::Eventing::Event<> ClickedEvent;
		VisionTools::Eventing::Event<> DoubleClickedEvent;
		VisionTools::Eventing::Event<> OpenedEvent;
		VisionTools::Eventing::Event<> ClosedEvent;

	private:
		bool m_arrowClickToOpen = false;
		bool m_shouldOpen = false;
		bool m_shouldClose = false;
		bool m_opened = false;
	};
}