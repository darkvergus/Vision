#include "VisionUI/Panels/PanelMenuBar.h"

void VisionUI::Panels::PanelMenuBar::_Draw_Impl()
{
	if (!m_widgets.empty() && ImGui::BeginMainMenuBar())
	{
		DrawWidgets();
		ImGui::EndMainMenuBar();
	}
}