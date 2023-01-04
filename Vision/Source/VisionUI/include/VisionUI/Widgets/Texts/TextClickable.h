#pragma once

#include <VisionTools/Eventing/Event.h>

#include "VisionUI/Widgets/Texts/Text.h"

namespace VisionUI::Widgets::Texts
{
	/**
	* Widget to display text on a panel that is also clickable
	*/
	class TextClickable : public Text
	{
	public:
		/**
		* Constructor
		* @param p_content
		*/
		TextClickable(const std::string& p_content = "");

	protected:
		virtual void _Draw_Impl() override;

	public:
		VisionTools::Eventing::Event<> ClickedEvent;
		VisionTools::Eventing::Event<> DoubleClickedEvent;
	};
}