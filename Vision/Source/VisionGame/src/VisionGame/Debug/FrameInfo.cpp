#ifdef _DEBUG

#include "VisionGame/Debug/FrameInfo.h"

VisionGame::Debug::FrameInfo::FrameInfo(VisionRendering::Core::Renderer& p_renderer, VisionWindowing::Window& p_window) :
	m_renderer(p_renderer),
	m_window(p_window)
{
	m_defaultHorizontalAlignment = VisionUI::Settings::EHorizontalAlignment::LEFT;
	m_defaultVerticalAlignment = VisionUI::Settings::EVerticalAlignment::BOTTOM;
	m_defaultPosition.x = static_cast<float>(p_window.GetSize().first) - 10.f;
	m_defaultPosition.y = static_cast<float>(p_window.GetSize().second) - 10.f;

	m_frameInfo[0] = &CreateWidget<VisionUI::Widgets::Texts::TextColored>("", VisionUI::Types::Color::Yellow);
	m_frameInfo[1] = &CreateWidget<VisionUI::Widgets::Texts::TextColored>("", VisionUI::Types::Color::Yellow);
	m_frameInfo[2] = &CreateWidget<VisionUI::Widgets::Texts::TextColored>("", VisionUI::Types::Color::Yellow);
}

void VisionGame::Debug::FrameInfo::Update(float p_deltaTime)
{
	auto& frameInfo = m_renderer.GetFrameInfo();

	m_frameInfo[0]->content = "Triangles: " + std::to_string(frameInfo.polyCount);
	m_frameInfo[1]->content = "Batches: " + std::to_string(frameInfo.batchCount);
	m_frameInfo[2]->content = "Instances: " + std::to_string(frameInfo.instanceCount);

	SetPosition({ 10.0f , static_cast<float>(m_window.GetSize().second) - 10.f });
	SetAlignment(VisionUI::Settings::EHorizontalAlignment::LEFT, VisionUI::Settings::EVerticalAlignment::BOTTOM);
}

#endif