#include "VisionUI/Widgets/Texts/TextLabelled.h"

VisionUI::Widgets::Texts::TextLabelled::TextLabelled(const std::string& p_content, const std::string& p_label) :
	Text(p_content), label(p_label)
{
}

void VisionUI::Widgets::Texts::TextLabelled::_Draw_Impl()
{
	ImGui::LabelText((label + m_widgetID).c_str(), content.c_str());
}
