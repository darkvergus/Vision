#include "VisionUI/Panels/APanelTransformable.h"
#include "VisionUI/Internal/Converter.h"

VisionUI::Panels::APanelTransformable::APanelTransformable
(
	const VisionMaths::FVector2& p_defaultPosition,
	const VisionMaths::FVector2& p_defaultSize,
	Settings::EHorizontalAlignment p_defaultHorizontalAlignment,
	Settings::EVerticalAlignment p_defaultVerticalAlignment,
	bool p_ignoreConfigFile
) :
	m_defaultPosition(p_defaultPosition),
	m_defaultSize(p_defaultSize),
	m_defaultHorizontalAlignment(p_defaultHorizontalAlignment),
	m_defaultVerticalAlignment(p_defaultVerticalAlignment),
	m_ignoreConfigFile(p_ignoreConfigFile)
{
}

void VisionUI::Panels::APanelTransformable::SetPosition(const VisionMaths::FVector2& p_position)
{
	m_position = p_position;
	m_positionChanged = true;
}

void VisionUI::Panels::APanelTransformable::SetSize(const VisionMaths::FVector2& p_size)
{
	m_size = p_size;
	m_sizeChanged = true;
}

void VisionUI::Panels::APanelTransformable::SetAlignment(Settings::EHorizontalAlignment p_horizontalAlignment, Settings::EVerticalAlignment p_verticalAligment)
{
	m_horizontalAlignment = p_horizontalAlignment;
	m_verticalAlignment = p_verticalAligment;
	m_alignmentChanged = true;
}

const VisionMaths::FVector2 & VisionUI::Panels::APanelTransformable::GetPosition() const
{
	return m_position;
}

const VisionMaths::FVector2 & VisionUI::Panels::APanelTransformable::GetSize() const
{
	return m_size;
}

VisionUI::Settings::EHorizontalAlignment VisionUI::Panels::APanelTransformable::GetHorizontalAlignment() const
{
	return m_horizontalAlignment;
}

VisionUI::Settings::EVerticalAlignment VisionUI::Panels::APanelTransformable::GetVerticalAlignment() const
{
	return m_verticalAlignment;
}

void VisionUI::Panels::APanelTransformable::UpdatePosition()
{
	if (m_defaultPosition.x != -1.f && m_defaultPosition.y != 1.f)
	{
		VisionMaths::FVector2 offsettedDefaultPos = m_defaultPosition + CalculatePositionAlignmentOffset(true);
		ImGui::SetWindowPos(Internal::Converter::ToImVec2(offsettedDefaultPos), m_ignoreConfigFile ? ImGuiCond_Once : ImGuiCond_FirstUseEver);
	}

	if (m_positionChanged || m_alignmentChanged)
	{
		VisionMaths::FVector2 offset = CalculatePositionAlignmentOffset(false);
		VisionMaths::FVector2 offsettedPos(m_position.x + offset.x, m_position.y + offset.y);
		ImGui::SetWindowPos(Internal::Converter::ToImVec2(offsettedPos), ImGuiCond_Always);
		m_positionChanged = false;
		m_alignmentChanged = false;
	}
}

void VisionUI::Panels::APanelTransformable::UpdateSize()
{
	/*
	if (m_defaultSize.x != -1.f && m_defaultSize.y != 1.f)
		ImGui::SetWindowSize(Internal::Converter::ToImVec2(m_defaultSize), m_ignoreConfigFile ? ImGuiCond_Once : ImGuiCond_FirstUseEver);
	*/
	if (m_sizeChanged)
	{
		ImGui::SetWindowSize(Internal::Converter::ToImVec2(m_size), ImGuiCond_Always);
		m_sizeChanged = false;
	}
}

void VisionUI::Panels::APanelTransformable::CopyImGuiPosition()
{
	m_position = Internal::Converter::ToFVector2(ImGui::GetWindowPos());
}

void VisionUI::Panels::APanelTransformable::CopyImGuiSize()
{
	m_size = Internal::Converter::ToFVector2(ImGui::GetWindowSize());
}

void VisionUI::Panels::APanelTransformable::Update()
{
	if (!m_firstFrame)
	{
		if (!autoSize)
			UpdateSize();
		CopyImGuiSize();

		UpdatePosition();
		CopyImGuiPosition();
	}

	m_firstFrame = false;
}

VisionMaths::FVector2 VisionUI::Panels::APanelTransformable::CalculatePositionAlignmentOffset(bool p_default)
{
	VisionMaths::FVector2 result(0.0f, 0.0f);

	switch (p_default ? m_defaultHorizontalAlignment : m_horizontalAlignment)
	{
	case VisionUI::Settings::EHorizontalAlignment::CENTER:
		result.x -= m_size.x / 2.0f;
		break;
	case VisionUI::Settings::EHorizontalAlignment::RIGHT:
		result.x -= m_size.x;
		break;
	}

	switch (p_default ? m_defaultVerticalAlignment : m_verticalAlignment)
	{
	case VisionUI::Settings::EVerticalAlignment::MIDDLE:
		result.y -= m_size.y / 2.0f;
		break;
	case VisionUI::Settings::EVerticalAlignment::BOTTOM:
		result.y -= m_size.y;
		break;
	}

	return result;
}
