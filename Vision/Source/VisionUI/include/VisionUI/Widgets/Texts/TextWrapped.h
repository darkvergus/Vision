#pragma once

#include "VisionUI/Widgets/Texts/Text.h"

namespace VisionUI::Widgets::Texts
{
	/**
	* Simple widget to display a long text with word-wrap on a panel
	*/
	class TextWrapped : public Text
	{
	public:
		/**
		* Constructor
		* @param p_content
		*/
		TextWrapped(const std::string& p_content = "");

	protected:
		virtual void _Draw_Impl() override;
	};
}