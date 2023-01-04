#include "VisionAudio/Tracking/SoundTracker.h"

VisionAudio::Tracking::SoundTracker::SoundTracker(irrklang::ISound* p_track) :
	m_track(p_track),
	m_soundStopEventBinder(StopEvent)
{
	// m_track->setSoundStopEventReceiver(&m_soundStopEventBinder);
}

VisionAudio::Tracking::SoundTracker::~SoundTracker()
{
	// m_track->setSoundStopEventReceiver(nullptr);
}

irrklang::ISound* VisionAudio::Tracking::SoundTracker::GetTrack() const
{
	return m_track;
}
