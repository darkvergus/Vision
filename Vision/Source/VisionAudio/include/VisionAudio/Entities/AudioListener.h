#pragma once


#include "VisionMaths/FVector3.h"
#include "VisionMaths/FTransform.h"

#include "VisionAudio/Entities/AudioSource.h"

namespace VisionAudio::Entities
{
	/**
	* Represents the ears of your application.
	* You can have multiple ones but only the last created will be considered by the AudioEngine
	*/
	class AudioListener
	{
	public:
		/**
		* AudioListener constructor (Internal transform management)
		*/
		AudioListener();

		/**
		* AudioListener constructor (External transform management)
		* @param p_transform
		*/
		AudioListener(VisionMaths::FTransform& p_transform);

		/**
		* AudioListener destructor
		*/
		~AudioListener();

		/**
		* Returns the AudioListener FTransform
		*/
		VisionMaths::FTransform& GetTransform();

		/**
		* Enable or disable the audio listener
		* @param p_enable
		*/
		void SetEnabled(bool p_enable);

		/**
		* Returns true if the audio listener is enabled
		*/
		bool IsEnabled() const;

	private:
		void Setup();

	private:
		/* Transform stuff */
		VisionMaths::FTransform* const		m_transform;
		const bool						m_internalTransform;
		bool							m_enabled = true;

	public:
		static VisionTools::Eventing::Event<AudioListener&>	CreatedEvent;
		static VisionTools::Eventing::Event<AudioListener&>	DestroyedEvent;
	};
}