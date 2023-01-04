#pragma once

#ifdef _DEBUG

#include <VisionRendering/Core/Renderer.h>
#include <VisionWindowing/Window.h>

#include <VisionAnalytics/Profiling/Profiler.h>
#include <VisionUI/Panels/PanelUndecorated.h>
#include <VisionUI/Widgets/Texts/TextColored.h>
#include <VisionUI/Widgets/Layout/Group.h>
#include <VisionUI/Widgets/Buttons/Button.h>

namespace VisionGame::Debug
{
	/**
	* Panel that display profiling information
	*/
	class GameProfiler : public VisionUI::Panels::PanelUndecorated
	{
	public:
		/**
		* Constructor
		* @param p_window
		* @param p_frequency
		*/
		GameProfiler(VisionWindowing::Window& p_window, float p_frequency);

		/**
		* Update the data
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);
	

	private:
		VisionUI::Types::Color CalculateActionColor(double p_percentage) const;
		std::string GenerateActionString(VisionAnalytics::Profiling::ProfilerReport::Action& p_action);

	private:

		float m_frequency;
		float m_timer = 0.f;

		VisionAnalytics::Profiling::Profiler m_profiler;

		VisionWindowing::Window& m_window;
		VisionUI::Widgets::AWidget* m_separator;
		VisionUI::Widgets::Texts::TextColored* m_elapsedFramesText;
		VisionUI::Widgets::Texts::TextColored* m_elapsedTimeText;
		VisionUI::Widgets::Layout::Group* m_actionList;
	};
}

#endif