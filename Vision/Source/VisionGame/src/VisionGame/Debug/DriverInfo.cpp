#ifdef _DEBUG

#include "VisionGame/Debug/DriverInfo.h"

VisionGame::Debug::DriverInfo::DriverInfo(VisionRendering::Core::Renderer& p_renderer, VisionWindowing::Window& p_window)
{
	m_defaultHorizontalAlignment = VisionUI::Settings::EHorizontalAlignment::RIGHT;
	m_defaultVerticalAlignment = VisionUI::Settings::EVerticalAlignment::BOTTOM;
	m_defaultPosition.x = static_cast<float>(p_window.GetSize().first) - 10.f;
	m_defaultPosition.y = static_cast<float>(p_window.GetSize().second) - 10.f;

	CreateWidget<VisionUI::Widgets::Texts::TextColored>("Vendor: "	+ p_renderer.GetString(GL_VENDOR), VisionUI::Types::Color::Yellow);
	CreateWidget<VisionUI::Widgets::Texts::TextColored>("Hardware: " + p_renderer.GetString(GL_RENDERER), VisionUI::Types::Color::Yellow);
	CreateWidget<VisionUI::Widgets::Texts::TextColored>("OpenGL Version: " + p_renderer.GetString(GL_VERSION), VisionUI::Types::Color::Yellow);
	CreateWidget<VisionUI::Widgets::Texts::TextColored>("GLSL Version: " + p_renderer.GetString(GL_SHADING_LANGUAGE_VERSION), VisionUI::Types::Color::Yellow);
}

#endif
