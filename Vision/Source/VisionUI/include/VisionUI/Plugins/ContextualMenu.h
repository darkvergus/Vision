#pragma once

#include "VisionUI/Plugins/IPlugin.h"
#include "VisionUI/Internal/WidgetContainer.h"
#include "VisionUI/Widgets/Menu/MenuList.h"
#include "VisionUI/Widgets/Menu/MenuItem.h"

namespace VisionUI::Plugins
{
	/**
	* A simple plugin that will show a contextual menu on right click
	* You can add widgets to a contextual menu
	*/
	class ContextualMenu : public IPlugin, public Internal::WidgetContainer
	{
	public:
		/**
		* Execute the plugin
		*/
		void Execute();

		/**
		* Force close the contextual menu
		*/
		void Close();
	};
}
