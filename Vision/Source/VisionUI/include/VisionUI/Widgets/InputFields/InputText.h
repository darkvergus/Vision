#pragma once

#include <VisionTools/Eventing/Event.h>

#include "VisionUI/Widgets/DataWidget.h"

namespace VisionUI::Widgets::InputFields
{
	/**
	* Input widget of type string
	*/
	class InputText : public DataWidget<std::string>
	{
	public:
		/**
		* Constructor
		* @param p_content
		* @param p_label
		*/
		InputText(const std::string& p_content = "", const std::string& p_label = "");

	protected:
		void _Draw_Impl() override;

	public:
		std::string content;
		std::string label;
		bool selectAllOnClick = false;
		VisionTools::Eventing::Event<std::string> ContentChangedEvent;
		VisionTools::Eventing::Event<std::string> EnterPressedEvent;
	};
}