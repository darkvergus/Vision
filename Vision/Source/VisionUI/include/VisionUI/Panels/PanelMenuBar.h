#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include <VisionTools/Eventing/Event.h>

#include "VisionUI/Panels/APanel.h"
#include "VisionUI/Widgets/Menu/MenuList.h"

namespace VisionUI::Panels
{
	/**
	* A simple panel that will be displayed on the top side of the canvas
	*/
	class PanelMenuBar : public APanel
	{
	protected:
		void _Draw_Impl() override;
	};
}