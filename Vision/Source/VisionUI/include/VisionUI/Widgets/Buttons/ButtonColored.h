#pragma once

#include <string>

#include <VisionMaths/FVector2.h>

#include "VisionUI/Types/Color.h"
#include "VisionUI/Widgets/Buttons/AButton.h"

namespace VisionUI::Widgets::Buttons
{
	/**
	* Button widget of a single color (Color palette element)
	*/
	class ButtonColored : public AButton
	{
	public:
		/**
		* Constructor
		* @param p_label
		* @param p_color
		* @param p_size
		* @param p_enableAlpha
		*/
		ButtonColored(const std::string& p_label = "", const Types::Color& p_color = {}, const VisionMaths::FVector2& p_size =VisionMaths::FVector2(0.f, 0.f), bool p_enableAlpha = true);

	protected:
		void _Draw_Impl() override;

	public:
		std::string label;
		Types::Color color;
		VisionMaths::FVector2 size;
		bool enableAlpha;
	};
}