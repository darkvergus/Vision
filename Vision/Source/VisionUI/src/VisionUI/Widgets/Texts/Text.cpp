#include "VisionUI/Widgets/Texts/Text.h"

VisionUI::Widgets::Texts::Text::Text(const std::string & p_content) :
	DataWidget(content),
	content(p_content)
{
}

void VisionUI::Widgets::Texts::Text::_Draw_Impl()
{
	ImGui::Text(content.c_str());
}
