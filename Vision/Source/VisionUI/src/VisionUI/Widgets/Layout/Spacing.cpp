#include "VisionUI/Widgets/Layout/Spacing.h"

VisionUI::Widgets::Layout::Spacing::Spacing(uint16_t p_spaces) : spaces(p_spaces)
{
}

void VisionUI::Widgets::Layout::Spacing::_Draw_Impl()
{
	for (uint16_t i = 0; i < spaces; ++i)
	{
		ImGui::Spacing();

		if (i + 1 < spaces)
			ImGui::SameLine();
	}
}
