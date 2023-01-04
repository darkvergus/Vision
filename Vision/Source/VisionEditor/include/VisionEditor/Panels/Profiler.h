#pragma once

#include <VisionAnalytics/Profiling/Profiler.h>

#include <VisionUI/Panels/PanelWindow.h>
#include <VisionUI/Widgets/Texts/TextColored.h>
#include <VisionUI/Widgets/Selection/CheckBox.h>
#include <VisionUI/Widgets/Layout/Group.h>
#include <VisionUI/Widgets/Layout/Columns.h>
#include <VisionUI/Widgets/Buttons/Button.h>

namespace VisionEditor::Panels
{
	class Profiler : public VisionUI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		* @param p_frequency
		*/
		Profiler
		(
			const std::string& p_title,
			bool p_opened,
			const VisionUI::Settings::PanelWindowSettings& p_windowSettings,
			float p_frequency
		);

		/**
		* Update profiling information
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);

		/**
		* Enable or disable the profiler
		* @param p_value
		* @param p_disableLog
		*/
		void Enable(bool p_value, bool p_disableLog = false);

	private:
		VisionUI::Types::Color CalculateActionColor(double p_percentage) const;
		std::string GenerateActionString(VisionAnalytics::Profiling::ProfilerReport::Action& p_action);

	private:
		enum class EProfilingMode
		{
			DEFAULT,
			CAPTURE
		};

		float m_frequency;
		float m_timer = 0.f;
		float m_fpsTimer = 0.f;
		EProfilingMode m_profilingMode = EProfilingMode::DEFAULT;

		VisionAnalytics::Profiling::Profiler m_profiler;

		VisionUI::Widgets::AWidget* m_separator;
		VisionUI::Widgets::Buttons::Button* m_captureResumeButton;
		VisionUI::Widgets::Texts::TextColored* m_fpsText;
		VisionUI::Widgets::Texts::TextColored* m_elapsedFramesText;
		VisionUI::Widgets::Texts::TextColored* m_elapsedTimeText;
		VisionUI::Widgets::Layout::Columns<5>* m_actionList;
	};
}