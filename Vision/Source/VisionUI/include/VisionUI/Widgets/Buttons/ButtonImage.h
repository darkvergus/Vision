#pragma once

#include <VisionMaths/FVector2.h>

#include "VisionUI/Internal/TextureID.h"
#include "VisionUI/Types/Color.h"
#include "VisionUI/Widgets/Buttons/AButton.h"

namespace VisionUI::Widgets::Buttons
{
	/**
	* Button widget with an image
	*/
	class ButtonImage : public AButton
	{
	public:
		/**
		* Constructor
		* @param p_textureID
		* @param p_size
		*/
		ButtonImage(uint32_t p_textureID, const VisionMaths::FVector2& p_size);

	protected:
		void _Draw_Impl() override;

	public:
		bool disabled = false;

		VisionUI::Types::Color background = { 0, 0, 0, 0 };
		VisionUI::Types::Color tint = { 1, 1, 1, 1 };

		Internal::TextureID textureID;
		VisionMaths::FVector2 size;
	};
}