#include "VisionUI/Widgets/Texts/TextDisabled.h"

VisionUI::Widgets::Texts::TextDisabled::TextDisabled(const std::string & p_content) :
	Text(p_content)
{
}

void VisionUI::Widgets::Texts::TextDisabled::_Draw_Impl()
{
	ImGui::TextDisabled(content.c_str());
}
