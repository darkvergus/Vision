#ifdef _DEBUG

#include "VisionGame/Debug/GameProfiler.h"

#include <VisionDebug/Logger.h>
#include <VisionUI/Widgets/Visual/Separator.h>
#include <VisionAnalytics/Profiling/ProfilerSpy.h>

using namespace VisionUI::Panels;
using namespace VisionUI::Widgets;
using namespace VisionUI::Types;

VisionGame::Debug::GameProfiler::GameProfiler(VisionWindowing::Window& p_window, float p_frequency) : m_frequency(p_frequency), m_window(p_window)
{
	m_defaultHorizontalAlignment = VisionUI::Settings::EHorizontalAlignment::LEFT;
	m_defaultPosition = { 10.0f, 10.0f };

	CreateWidget<Texts::Text>("Profiler state: ").lineBreak = true;

	m_elapsedFramesText = &CreateWidget<Texts::TextColored>("", Color(1.f, 0.8f, 0.01f, 1));
	m_elapsedTimeText = &CreateWidget<Texts::TextColored>("", Color(1.f, 0.8f, 0.01f, 1));
	m_separator = &CreateWidget<VisionUI::Widgets::Visual::Separator>();
	m_actionList = &CreateWidget<Layout::Group>();

	m_actionList->CreateWidget<Texts::Text>("Action | Total duration | Frame Duration | Frame load | Total calls");

	m_profiler.Enable();

	m_elapsedFramesText->enabled = true;
	m_elapsedTimeText->enabled = true;
	m_separator->enabled = true;
}

void VisionGame::Debug::GameProfiler::Update(float p_deltaTime)
{
	PROFILER_SPY("Game Profiler Update");

	m_position = { 10.0f, static_cast<float>(m_window.GetSize().second / 2) };

	m_timer += p_deltaTime;

	if (m_profiler.IsEnabled())
	{
		m_profiler.Update(p_deltaTime);

		while (m_timer >= m_frequency)
		{
			
			VisionAnalytics::Profiling::ProfilerReport report = m_profiler.GenerateReport();
			m_profiler.ClearHistory();
			m_actionList->RemoveAllWidgets();

			m_elapsedFramesText->content = "Elapsed frames: " + std::to_string(report.elapsedFrames);
			m_elapsedTimeText->content = "Elapsed time: " + std::to_string(report.elaspedTime);

			m_actionList->CreateWidget<Texts::Text>("Action | Total duration | Frame Duration | Frame load | Total calls");

			for (auto& action : report.actions)
			{
				auto color = CalculateActionColor(action.percentage);
				m_actionList->CreateWidget<Texts::TextColored>(GenerateActionString(action), color);
			}
		
			m_timer -= m_frequency;
		}
	}
}

VisionUI::Types::Color VisionGame::Debug::GameProfiler::CalculateActionColor(double p_percentage) const
{
	if (p_percentage <= 25.0f)		return { 0.0f, 1.0f, 0.0f, 1.0f };
	else if (p_percentage <= 50.0f) return { 1.0f, 1.0f, 0.0f, 1.0f };
	else if (p_percentage <= 75.0f) return { 1.0f, 0.6f, 0.0f, 1.0f };
	else							return { 1.0f, 0.0f, 0.0f, 1.0f };
}

std::string VisionGame::Debug::GameProfiler::GenerateActionString(VisionAnalytics::Profiling::ProfilerReport::Action & p_action)
{
	std::string result;

	result += "[" + p_action.name + "]";
	result += std::to_string(p_action.duration) + "s (total) | ";
	result += std::to_string(p_action.duration / p_action.calls) + "s (per call) | ";
	result += std::to_string(p_action.percentage) + "%% | ";
	result += std::to_string(p_action.calls) + " calls";

	return result;
}

#endif