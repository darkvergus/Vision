#include "VisionAudio/Tracking/SoundStopEventBinder.h"

VisionAudio::Tracking::SoundStopEventBinder::SoundStopEventBinder(VisionTools::Eventing::Event<>& p_soundFinishedEvent) : SoundFinishedEvent(p_soundFinishedEvent)
{
}

void VisionAudio::Tracking::SoundStopEventBinder::OnSoundStopped(irrklang::ISound * sound, irrklang::E_STOP_EVENT_CAUSE reason, void * userData)
{
	SoundFinishedEvent.Invoke();
}
