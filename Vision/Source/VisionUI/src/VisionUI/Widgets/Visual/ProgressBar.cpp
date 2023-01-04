#include "VisionUI/Widgets/Visual/ProgressBar.h"
#include "VisionUI/Internal/Converter.h"

VisionUI::Widgets::Visual::ProgressBar::ProgressBar(float p_fraction, const VisionMaths::FVector2 & p_size, const std::string & p_overlay) :
	fraction(p_fraction), size(p_size), overlay(p_overlay)
{
}

void VisionUI::Widgets::Visual::ProgressBar::_Draw_Impl()
{
	ImGui::ProgressBar(fraction, Internal::Converter::ToImVec2(size), !overlay.empty() ? overlay.c_str() : nullptr);
}
