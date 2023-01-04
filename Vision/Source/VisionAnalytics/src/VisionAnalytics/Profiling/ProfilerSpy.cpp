#include "VisionAnalytics/Profiling/ProfilerSpy.h"
#include "VisionAnalytics/Profiling/Profiler.h"

VisionAnalytics::Profiling::ProfilerSpy::ProfilerSpy(const std::string & p_name) :
	name(p_name),
	start(std::chrono::steady_clock::now())
{

}

VisionAnalytics::Profiling::ProfilerSpy::~ProfilerSpy()
{
	end = std::chrono::steady_clock::now();
	VisionAnalytics::Profiling::Profiler::Save(*this);
}