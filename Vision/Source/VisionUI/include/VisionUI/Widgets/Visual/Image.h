#pragma once

#include <VisionMaths/FVector2.h>

#include "VisionUI/Internal/TextureID.h"
#include "VisionUI/Widgets/AWidget.h"

namespace VisionUI::Widgets::Visual
{
	/**
	* Simple widget that display an image
	*/
	class Image : public AWidget
	{
	public:
		/**
		* Constructor
		* @param p_textureID
		* @parma p_size
		*/
		Image(uint32_t p_textureID, const VisionMaths::FVector2& p_size);

	protected:
		void _Draw_Impl() override;

	public:
		Internal::TextureID textureID;
		VisionMaths::FVector2 size;
	};
}