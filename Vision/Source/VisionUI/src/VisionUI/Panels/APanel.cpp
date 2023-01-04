#include <algorithm>

#include "VisionUI/Panels/APanel.h"

uint64_t VisionUI::Panels::APanel::__PANEL_ID_INCREMENT = 0;

VisionUI::Panels::APanel::APanel()
{
	m_panelID = "##" + std::to_string(__PANEL_ID_INCREMENT++);
}

void VisionUI::Panels::APanel::Draw()
{
	if (enabled)
		_Draw_Impl();
}

const std::string & VisionUI::Panels::APanel::GetPanelID() const
{
	return m_panelID;
}
