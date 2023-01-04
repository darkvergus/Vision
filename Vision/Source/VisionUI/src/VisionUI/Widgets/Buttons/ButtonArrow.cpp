#include "VisionUI/Widgets/Buttons/ButtonArrow.h"

VisionUI::Widgets::Buttons::ButtonArrow::ButtonArrow(ImGuiDir p_direction) :
	direction(p_direction)
{
}

void VisionUI::Widgets::Buttons::ButtonArrow::_Draw_Impl()
{
	if (ImGui::ArrowButton(m_widgetID.c_str(), direction))
		ClickedEvent.Invoke();
}
