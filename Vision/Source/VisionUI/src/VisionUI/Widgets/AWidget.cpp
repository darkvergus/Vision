#include "VisionUI/Widgets/AWidget.h"

uint64_t VisionUI::Widgets::AWidget::__WIDGET_ID_INCREMENT = 0;

VisionUI::Widgets::AWidget::AWidget()
{
	m_widgetID = "##" + std::to_string(__WIDGET_ID_INCREMENT++);
}

void VisionUI::Widgets::AWidget::LinkTo(const AWidget& p_widget)
{
	m_widgetID = p_widget.m_widgetID;
}

void VisionUI::Widgets::AWidget::Destroy()
{
	m_destroyed = true;
}

bool VisionUI::Widgets::AWidget::IsDestroyed() const
{
	return m_destroyed;
}

void VisionUI::Widgets::AWidget::SetParent(Internal::WidgetContainer * p_parent)
{
	m_parent = p_parent;
}

bool VisionUI::Widgets::AWidget::HasParent() const
{
	return m_parent;
}

VisionUI::Internal::WidgetContainer * VisionUI::Widgets::AWidget::GetParent()
{
	return m_parent;
}

void VisionUI::Widgets::AWidget::Draw()
{
	if (enabled)
	{
		_Draw_Impl();

		if (m_autoExecutePlugins)
			ExecutePlugins();

		if (!lineBreak)
			ImGui::SameLine();
	}
}
