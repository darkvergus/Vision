#include "VisionUI/Widgets/Layout/Dummy.h"
#include "VisionUI/Internal/Converter.h"

VisionUI::Widgets::Layout::Dummy::Dummy(const VisionMaths::FVector2& p_size) : size(p_size)
{
}

void VisionUI::Widgets::Layout::Dummy::_Draw_Impl()
{
	ImGui::Dummy(Internal::Converter::ToImVec2(size));
}
