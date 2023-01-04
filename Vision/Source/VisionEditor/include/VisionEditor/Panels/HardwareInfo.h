#pragma once

#include <VisionUI/Panels/PanelWindow.h>
#include <VisionUI/Widgets/Plots/PlotLines.h>
#include <VisionUI/Widgets/Plots/PlotHistogram.h>

namespace VisionAnalytics::Hardware { class HardwareInfo; }

namespace VisionEditor::Panels
{
	class HardwareInfo : public VisionUI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		* @param p_logFrequency
		* @param p_maxElements
		* @param p_displayMode
		*/
		HardwareInfo
		(
			const std::string& p_title,
			bool p_opened,
			const VisionUI::Settings::PanelWindowSettings& p_windowSettings,
			float p_logFrequency,
			size_t p_maxElements
		);

		/**
		* Destructor
		*/
		~HardwareInfo();

		/**
		* Update hardware info
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);

	private:
		float p_updateTimer = 0.f;
		float m_logFrequency;
		size_t m_maxElements;
		VisionUI::Widgets::Plots::APlot* m_cpuUsage;
		VisionUI::Widgets::Plots::APlot* m_gpuUsage;
		VisionUI::Widgets::Plots::APlot* m_ramUsage;
		VisionAnalytics::Hardware::HardwareInfo* m_hardwareInfo;
	};
}