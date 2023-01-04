#pragma once

#include "VisionUI/Panels/APanelTransformable.h"

namespace VisionUI::Panels
{
	/**
	* A simple panel that is transformable and without decorations (No background)
	*/
	class PanelUndecorated : public APanelTransformable
	{
	public:
		void _Draw_Impl() override;

	private:
		int CollectFlags();
	};
}