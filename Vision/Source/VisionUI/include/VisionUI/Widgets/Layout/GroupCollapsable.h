#pragma once

#include <vector>

#include <VisionTools/Eventing/Event.h>

#include "VisionUI/Widgets/Layout/Group.h"

namespace VisionUI::Widgets::Layout
{
	/**
	* Widget that can contains other widgets and is collapsable
	*/
	class GroupCollapsable : public Group
	{
	public:
		/**
		* Constructor
		* @param p_name
		*/
		GroupCollapsable(const std::string& p_name = "");

	protected:
		virtual void _Draw_Impl() override;

	public:
		std::string name;
		bool closable = false;
		bool opened = true;
		VisionTools::Eventing::Event<> CloseEvent;
		VisionTools::Eventing::Event<> OpenEvent;
	};
}