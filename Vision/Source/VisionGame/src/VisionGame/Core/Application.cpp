#include <VisionTools/Time/Clock.h>

#include "VisionGame/Core/Application.h"

VisionGame::Core::Application::Application() :
	m_game(m_context)
{

}

VisionGame::Core::Application::~Application()
{
}

void VisionGame::Core::Application::Run()
{
	VisionTools::Time::Clock clock;

	while (IsRunning())
	{
		m_game.PreUpdate();
		m_game.Update(clock.GetDeltaTime());
		m_game.PostUpdate();

		clock.Update();
	}
}

bool VisionGame::Core::Application::IsRunning() const
{
	return !m_context.window->ShouldClose();
}
