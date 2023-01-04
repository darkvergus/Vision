#pragma once

#include "VisionUI/Widgets/Texts/Text.h"

namespace VisionUI::Widgets::Texts
{
	/**
	* Widget to display a disabled text on a panel
	*/
	class TextDisabled : public Text
	{
	public:
		/**
		* Constructor
		* @param p_content
		*/
		TextDisabled(const std::string& p_content = "");

	protected:
		virtual void _Draw_Impl() override;
	};
}