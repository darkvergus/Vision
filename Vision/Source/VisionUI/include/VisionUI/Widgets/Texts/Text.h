#pragma once

#include "VisionUI/Widgets/AWidget.h"
#include "VisionUI/Widgets/DataWidget.h"

namespace VisionUI::Widgets::Texts
{
	/**
	* Simple widget to display text on a panel
	*/
	class Text : public DataWidget<std::string>
	{
	public:
		/**
		* Constructor
		* @param p_content
		*/
		Text(const std::string& p_content = "");

	protected:
		virtual void _Draw_Impl() override;

	public:
		std::string content;
	};
}