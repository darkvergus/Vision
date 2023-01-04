#include "VisionAnalytics/Hardware/RAMInfo.h"

void VisionAnalytics::Hardware::RAMInfo::Update()
{
	m_statex.dwLength = sizeof(m_statex);
	GlobalMemoryStatusEx(&m_statex);
}

float VisionAnalytics::Hardware::RAMInfo::GetUsedRAM()
{
	return GetMaxRAM() - m_statex.ullAvailPhys / 1048576.0f;
}

float VisionAnalytics::Hardware::RAMInfo::GetFreeRAM()
{
	return m_statex.ullAvailPhys / 1048576.0f;
}

float VisionAnalytics::Hardware::RAMInfo::GetMaxRAM()
{
	return m_statex.ullTotalPhys / 1048576.0f;
}