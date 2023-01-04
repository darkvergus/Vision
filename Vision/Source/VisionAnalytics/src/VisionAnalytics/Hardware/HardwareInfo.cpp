#include <Windows.h>

#include "VisionAnalytics/Hardware/HardwareInfo.h"

VisionAnalytics::Hardware::HardwareInfo::HardwareInfo(double p_timeInterval) :
	m_timeInterval(p_timeInterval),
	m_timer(0.0f),
	m_cpuUsage(0.0f),
	m_gpuUsage(0.0f)
{
}

VisionAnalytics::Hardware::HardwareReport VisionAnalytics::Hardware::HardwareInfo::GenerateReport()
{
	return HardwareReport
	{
		m_cpuUsage,
		m_gpuUsage,
		m_ramInfo.GetUsedRAM(),
		m_ramInfo.GetFreeRAM(),
		m_ramInfo.GetMaxRAM()
	};
}

void VisionAnalytics::Hardware::HardwareInfo::Tick()
{
	m_currentTime = std::chrono::steady_clock::now();
	m_elapsedTime = m_currentTime - m_previousTime;

	m_timer += m_elapsedTime.count();

	if (m_timer >= m_timeInterval)
	{
		while (m_timer >= m_timeInterval)
			m_timer -= m_timeInterval;

		Update();
	}

	m_previousTime = m_currentTime;
}

void VisionAnalytics::Hardware::HardwareInfo::Update()
{
	m_cpuUsage = m_cpuInfo.CalculateCPULoad();
	m_gpuUsage = m_gpuInfo.CalculateGPULoad();

	m_ramInfo.Update();
}
