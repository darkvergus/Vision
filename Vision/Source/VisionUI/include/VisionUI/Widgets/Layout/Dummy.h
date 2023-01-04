#pragma once

#include <VisionMaths/FVector2.h>

#include "VisionUI/Widgets/AWidget.h"

namespace VisionUI::Widgets::Layout
{
	/**
	* Dummy widget that takes the given size as space in the panel
	*/
	class Dummy : public AWidget
	{
	public:
		/**
		* Constructor
		* @param p_size
		*/
		Dummy(const VisionMaths::FVector2& p_size = { 0.0f, 0.0f });

	protected:
		void _Draw_Impl() override;

	public:
		VisionMaths::FVector2 size;
	};
}