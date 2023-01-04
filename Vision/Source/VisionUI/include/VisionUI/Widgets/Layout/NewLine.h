#pragma once

#include "VisionUI/Widgets/AWidget.h"

namespace VisionUI::Widgets::Layout
{
	/**
	* Widget that adds an empty line to the panel
	*/
	class NewLine : public AWidget
	{
	protected:
		void _Draw_Impl() override;
	};
}