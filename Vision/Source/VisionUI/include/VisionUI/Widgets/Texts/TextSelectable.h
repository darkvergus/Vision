#pragma once

#include <VisionTools/Eventing/Event.h>

#include "VisionUI/Widgets/Texts/Text.h"

namespace VisionUI::Widgets::Texts
{
	/**
	* Simple widget to display a selectable text on a panel
	*/
	class TextSelectable : public Text
	{
	public:
		/**
		* Constructor
		* @param p_content
		* @param p_selected
		* @param p_disabled
		*/
		TextSelectable(const std::string& p_content = "", bool p_selected = false, bool p_disabled = false);

	protected:
		virtual void _Draw_Impl() override;

	public:
		bool selected;
		bool disabled;

		VisionTools::Eventing::Event<bool> ClickedEvent;
		VisionTools::Eventing::Event<> SelectedEvent;
		VisionTools::Eventing::Event<> UnselectedEvent;
	};
}