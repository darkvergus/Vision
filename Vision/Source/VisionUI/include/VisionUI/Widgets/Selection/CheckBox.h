#pragma once

#include <VisionTools/Eventing/Event.h>

#include "VisionUI/Widgets/DataWidget.h"

namespace VisionUI::Widgets::Selection
{
	/**
	* Checkbox widget that can be checked or not
	*/
	class CheckBox : public DataWidget<bool>
	{
	public:
		/** 
		* Constructor
		* @param p_value
		* @param p_label
		*/
		CheckBox(bool p_value = false, const std::string& p_label = "");

	protected:
		void _Draw_Impl() override;

	public:
		bool value;
		std::string label;
		VisionTools::Eventing::Event<bool> ValueChangedEvent;
	};
}