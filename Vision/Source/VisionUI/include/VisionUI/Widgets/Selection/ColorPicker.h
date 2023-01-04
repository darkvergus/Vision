#pragma once

#include <VisionTools/Eventing/Event.h>

#include "VisionUI/Widgets/DataWidget.h"
#include "VisionUI/Types/Color.h"

namespace VisionUI::Widgets::Selection
{
	/**
	* Widget that allow the selection of a color with a color picker
	*/
	class ColorPicker : public DataWidget<Types::Color>
	{
	public:
		/**
		* Constructor
		* @param p_enableAlpha
		* @param p_defaultColor
		*/
		ColorPicker(bool p_enableAlpha = false, const Types::Color& p_defaultColor = {});

	protected:
		void _Draw_Impl() override;

	public:
		bool enableAlpha;
		Types::Color color;
		VisionTools::Eventing::Event<Types::Color&> ColorChangedEvent;
	};
}