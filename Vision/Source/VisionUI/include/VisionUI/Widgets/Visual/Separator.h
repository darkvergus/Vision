#pragma once

#include "VisionUI/Widgets/AWidget.h"

namespace VisionUI::Widgets::Visual
{
	/**
	* Simple widget that display a separator
	*/
	class Separator : public AWidget
	{
	protected:
		void _Draw_Impl() override;
	};
}