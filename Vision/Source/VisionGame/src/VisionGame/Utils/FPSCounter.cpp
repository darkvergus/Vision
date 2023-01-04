#include "VisionGame/Utils/FPSCounter.h"

VisionGame::Utils::FPSCounter::FPSCounter(VisionWindowing::Window& p_window) : m_window(p_window)
{
	m_text.color = VisionUI::Types::Color::Yellow;
	m_defaultHorizontalAlignment = VisionUI::Settings::EHorizontalAlignment::RIGHT;
	m_defaultPosition = { static_cast<float>(m_window.GetSize().first) - 10.0f , 10.0f };
	m_text.content = "999 FPS";
	ConsiderWidget(m_text, false);
}

void VisionGame::Utils::FPSCounter::Update(float p_deltaTime)
{
	m_elapsed += p_deltaTime;

	++m_frames;

	if (m_elapsed >= 0.1f)
	{
		m_text.content = std::to_string(static_cast<int>(1.f / (m_elapsed / static_cast<float>(m_frames)))) + " FPS";
		SetPosition({ static_cast<float>(m_window.GetSize().first) - 10.0f , 10.0f });
		SetAlignment(VisionUI::Settings::EHorizontalAlignment::RIGHT, VisionUI::Settings::EVerticalAlignment::TOP);

		m_elapsed = 0.f;
		m_frames = 0;
	}
}
