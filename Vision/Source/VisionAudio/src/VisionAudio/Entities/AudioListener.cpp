#include "VisionAudio/Entities/AudioListener.h"

VisionTools::Eventing::Event<VisionAudio::Entities::AudioListener&>	VisionAudio::Entities::AudioListener::CreatedEvent;
VisionTools::Eventing::Event<VisionAudio::Entities::AudioListener&>	VisionAudio::Entities::AudioListener::DestroyedEvent;

VisionAudio::Entities::AudioListener::AudioListener() :
	m_transform(new VisionMaths::FTransform()),
	m_internalTransform(true)
{
	Setup();
}

VisionAudio::Entities::AudioListener::AudioListener(VisionMaths::FTransform& p_transform) :
	m_transform(&p_transform),
	m_internalTransform(false)
{
	Setup();
}

VisionAudio::Entities::AudioListener::~AudioListener()
{
	DestroyedEvent.Invoke(*this);
}

VisionMaths::FTransform& VisionAudio::Entities::AudioListener::GetTransform()
{
	return *m_transform;
}

void VisionAudio::Entities::AudioListener::SetEnabled(bool p_enable)
{
	m_enabled = p_enable;
}

bool VisionAudio::Entities::AudioListener::IsEnabled() const
{
	return m_enabled;
}

void VisionAudio::Entities::AudioListener::Setup()
{
	CreatedEvent.Invoke(*this);
}
