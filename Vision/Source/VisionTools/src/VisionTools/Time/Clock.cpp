#include "VisionTools/Time/Clock.h"

float VisionTools::Time::Clock::GetFramerate()
{
	return 1.0f / (__DELTA_TIME);
}

float VisionTools::Time::Clock::GetDeltaTime()
{
	return __DELTA_TIME * __TIME_SCALE;
}

float VisionTools::Time::Clock::GetDeltaTimeUnscaled()
{
	return __DELTA_TIME;
}

float VisionTools::Time::Clock::GetTimeSinceStart()
{
	return __TIME_SINCE_START;
}

float VisionTools::Time::Clock::GetTimeScale()
{
	return __TIME_SCALE;
}

void VisionTools::Time::Clock::Scale(float p_coeff)
{
	__TIME_SCALE *= p_coeff;
}

void VisionTools::Time::Clock::SetTimeScale(float p_timeScale)
{
	__TIME_SCALE = p_timeScale;
}

void VisionTools::Time::Clock::Initialize()
{
	__DELTA_TIME = 0.0f;

	__START_TIME = std::chrono::steady_clock::now();
	__CURRENT_TIME = __START_TIME;
	__LAST_TIME = __START_TIME;

	__INITIALIZED = true;
}

void VisionTools::Time::Clock::Update()
{
	__LAST_TIME = __CURRENT_TIME;
	__CURRENT_TIME = std::chrono::steady_clock::now();
	__ELAPSED = __CURRENT_TIME - __LAST_TIME;

	if (__INITIALIZED)
	{
		__DELTA_TIME = __ELAPSED.count() > 0.1 ? 0.1f : static_cast<float>(__ELAPSED.count());
		__TIME_SINCE_START += __DELTA_TIME * __TIME_SCALE;
	}
	else
		Initialize();
}