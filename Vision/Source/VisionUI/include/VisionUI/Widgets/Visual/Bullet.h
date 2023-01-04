#pragma once

#include "VisionUI/Widgets/AWidget.h"

namespace VisionUI::Widgets::Visual
{
	/**
	* Simple widget that display a bullet point
	*/
	class Bullet : public AWidget
	{
	protected:
		virtual void _Draw_Impl() override;
	};
}