#pragma once

#include <VisionMaths/FVector2.h>

#include "VisionUI/Widgets/AWidget.h"

namespace VisionUI::Widgets::Visual
{
	/**
	* Simple widget that display a progress bar
	*/
	class ProgressBar : public AWidget
	{
	public:
		/**
		* Constructor
		* @param p_fraction
		* @param p_size
		* @param p_overlay
		*/
		ProgressBar(float p_fraction = 0.0f, const VisionMaths::FVector2& p_size = { 0.0f, 0.0f }, const std::string& p_overlay = "");

	protected:
		void _Draw_Impl() override;

	public:
		float fraction;
		VisionMaths::FVector2 size;
		std::string overlay;
	};
}