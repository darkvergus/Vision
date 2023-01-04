#pragma once

#include <string>

#include <VisionTools/Eventing/Event.h>

#include "VisionUI/Widgets/AWidget.h"

namespace VisionUI::Widgets::Buttons
{
	/**
	* Base class for any button widget
	*/
	class AButton : public AWidget
	{
	protected:
		void _Draw_Impl() override = 0;

	public:
		VisionTools::Eventing::Event<> ClickedEvent;
	};
}