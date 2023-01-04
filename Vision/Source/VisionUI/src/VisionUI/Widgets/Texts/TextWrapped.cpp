#include "VisionUI/Widgets/Texts/TextWrapped.h"

VisionUI::Widgets::Texts::TextWrapped::TextWrapped(const std::string & p_content) :
	Text(p_content)
{
}

void VisionUI::Widgets::Texts::TextWrapped::_Draw_Impl()
{
	ImGui::TextWrapped(content.c_str());
}
