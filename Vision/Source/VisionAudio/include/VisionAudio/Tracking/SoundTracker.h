#pragma once

#include <irrklang/irrKlang.h>


#include "VisionAudio/Tracking/SoundStopEventBinder.h"

namespace VisionAudio::Tracking
{
	/**
	* Track a playing sound and allow the modification of its settings
	*/
	class SoundTracker
	{
	public:
		/**
		* Constructor
		* @param p_track
		*/
		SoundTracker(irrklang::ISound* p_track);

		/**
		* Destructor
		*/
		~SoundTracker();

		/**
		* Returns the tracked sound instance
		*/
		irrklang::ISound* GetTrack() const;

	public:
		/**
		* FinishedEvent is called when the track get stopped/finished
		*/
		VisionTools::Eventing::Event<> StopEvent;

	private:
		irrklang::ISound* const m_track = nullptr;

	private:
		SoundStopEventBinder m_soundStopEventBinder;
	};
}