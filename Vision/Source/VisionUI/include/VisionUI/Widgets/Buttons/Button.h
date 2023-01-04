#pragma once

#include <string>

#include <VisionMaths/FVector2.h>

#include "VisionUI/Widgets/Buttons/AButton.h"
#include "VisionUI/Types/Color.h"

namespace VisionUI::Widgets::Buttons
{
	/**
	* Simple button widget
	*/
	class Button : public AButton
	{
	public:
		/**
		* Constructor
		* @param p_label
		* @param p_size
		* @param p_disabled
		*/
		Button(const std::string& p_label = "", const VisionMaths::FVector2& p_size = VisionMaths::FVector2(0.f, 0.f), bool p_disabled = false);

	protected:
		void _Draw_Impl() override;

	public:
		std::string label;
		VisionMaths::FVector2 size;
		bool disabled = false;

		Types::Color idleBackgroundColor;
		Types::Color hoveredBackgroundColor;
		Types::Color clickedBackgroundColor;

		Types::Color textColor;
	};
}