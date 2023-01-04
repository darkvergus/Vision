#include "VisionUI/Widgets/Selection/RadioButton.h"

VisionUI::Widgets::Selection::RadioButton::RadioButton(bool p_selected, const std::string & p_label) :
	DataWidget<bool>(m_selected), label(p_label)
{
	if (p_selected)
		Select();
}

void VisionUI::Widgets::Selection::RadioButton::Select()
{
	m_selected = true;
	ClickedEvent.Invoke(m_radioID);
}

bool VisionUI::Widgets::Selection::RadioButton::IsSelected() const
{
	return m_selected;
}

bool VisionUI::Widgets::Selection::RadioButton::GetRadioID() const
{
	return m_radioID;
}

void VisionUI::Widgets::Selection::RadioButton::_Draw_Impl()
{
	if (ImGui::RadioButton((label + m_widgetID).c_str(), m_selected))
	{
		ClickedEvent.Invoke(m_radioID);
		this->NotifyChange();
	}
}
