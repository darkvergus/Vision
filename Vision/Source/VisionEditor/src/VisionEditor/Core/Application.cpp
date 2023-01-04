#include <VisionTools/Time/Clock.h>

#include "VisionEditor/Core/Application.h"

VisionEditor::Core::Application::Application(const std::string& p_projectPath, const std::string& p_projectName) :
	m_context(p_projectPath, p_projectName),
	m_editor(m_context)
{
}

VisionEditor::Core::Application::~Application()
{
}

void VisionEditor::Core::Application::Run()
{
	VisionTools::Time::Clock clock;

	while (IsRunning())
	{
		m_editor.PreUpdate();
		m_editor.Update(clock.GetDeltaTime());
		m_editor.PostUpdate();

		clock.Update();
	}
}

bool VisionEditor::Core::Application::IsRunning() const
{
	return !m_context.window->ShouldClose();
}
