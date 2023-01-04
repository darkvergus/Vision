#pragma once

#include <vector>

#include "VisionUI/Internal/WidgetContainer.h"

namespace VisionUI::Widgets::Layout
{
	/**
	* Widget that can contains other widgets
	*/
	class Group : public AWidget, public Internal::WidgetContainer
	{
	protected:
		virtual void _Draw_Impl() override;
	};
}