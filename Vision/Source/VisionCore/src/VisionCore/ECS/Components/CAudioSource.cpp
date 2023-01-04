#include <VisionUI/Widgets/Plots/PlotLines.h>
#include <VisionUI/Widgets/Drags/DragFloat.h>
#include <VisionAudio/Core/AudioPlayer.h>

#include "VisionCore/ECS/Components/CAudioSource.h"
#include "VisionCore/ECS/Actor.h"
#include "VisionCore/Global/ServiceLocator.h"
#include "VisionCore/SceneSystem/SceneManager.h"

VisionCore::ECS::Components::CAudioSource::CAudioSource(ECS::Actor& p_owner) :
	AComponent(p_owner),
	m_audioSource(VisionCore::Global::ServiceLocator::Get<VisionAudio::Core::AudioPlayer>(), owner.transform.GetFTransform())
{
}

std::string VisionCore::ECS::Components::CAudioSource::GetName()
{
	return "Audio Source";
}

void VisionCore::ECS::Components::CAudioSource::SetSound(VisionAudio::Resources::Sound* p_sound)
{
	m_sound = p_sound;
}

void VisionCore::ECS::Components::CAudioSource::SetAutoplay(bool p_autoplay)
{
	m_autoPlay = p_autoplay;
}

void VisionCore::ECS::Components::CAudioSource::SetVolume(float p_volume)
{
	m_audioSource.SetVolume(p_volume);
}

void VisionCore::ECS::Components::CAudioSource::SetPan(float p_pan)
{
	m_audioSource.SetPan(p_pan);
}

void VisionCore::ECS::Components::CAudioSource::SetLooped(bool p_looped)
{
	m_audioSource.SetLooped(p_looped);
}

void VisionCore::ECS::Components::CAudioSource::SetPitch(float p_pitch)
{
	m_audioSource.SetPitch(p_pitch);
}

void VisionCore::ECS::Components::CAudioSource::SetSpatial(bool p_value)
{
	m_audioSource.SetSpatial(p_value);
}

void VisionCore::ECS::Components::CAudioSource::SetAttenuationThreshold(float p_distance)
{
	m_audioSource.SetAttenuationThreshold(p_distance);
}

VisionAudio::Resources::Sound* VisionCore::ECS::Components::CAudioSource::GetSound() const
{
	return m_sound;
}

bool VisionCore::ECS::Components::CAudioSource::IsAutoplayed() const
{
	return m_autoPlay;
}

float VisionCore::ECS::Components::CAudioSource::GetVolume() const
{
	return m_audioSource.GetVolume();
}

float VisionCore::ECS::Components::CAudioSource::GetPan() const
{
	return m_audioSource.GetPan();
}

bool VisionCore::ECS::Components::CAudioSource::IsLooped() const
{
	return m_audioSource.IsLooped();
}

float VisionCore::ECS::Components::CAudioSource::GetPitch() const
{
	return m_audioSource.GetPitch();
}

bool VisionCore::ECS::Components::CAudioSource::IsFinished() const
{
	return m_audioSource.IsFinished();
}

bool VisionCore::ECS::Components::CAudioSource::IsSpatial() const
{
	return m_audioSource.IsSpatial();
}

float VisionCore::ECS::Components::CAudioSource::GetAttenuationThreshold() const
{
	return m_audioSource.GetAttenuationThreshold();
}

void VisionCore::ECS::Components::CAudioSource::Play()
{
	if (owner.IsActive() && m_sound)
		m_audioSource.Play(*m_sound);
}

void VisionCore::ECS::Components::CAudioSource::Pause()
{
	if (owner.IsActive())
		m_audioSource.Pause();
}

void VisionCore::ECS::Components::CAudioSource::Resume()
{
	if (owner.IsActive())
		m_audioSource.Resume();
}

void VisionCore::ECS::Components::CAudioSource::Stop()
{
	if (owner.IsActive())
		m_audioSource.Stop();
}

void VisionCore::ECS::Components::CAudioSource::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	using namespace VisionCore::Helpers;

	Serializer::SerializeBoolean(p_doc, p_node, "autoplay", m_autoPlay);
	Serializer::SerializeBoolean(p_doc, p_node, "spatial", IsSpatial());
	Serializer::SerializeFloat(p_doc, p_node, "volume", GetVolume());
	Serializer::SerializeFloat(p_doc, p_node, "pan", GetPan());
	Serializer::SerializeBoolean(p_doc, p_node, "looped", IsLooped());
	Serializer::SerializeFloat(p_doc, p_node, "pitch", GetPitch());
	Serializer::SerializeFloat(p_doc, p_node, "attenuation_threshold", GetAttenuationThreshold());
	Serializer::SerializeSound(p_doc, p_node, "audio_clip", m_sound);
}

void VisionCore::ECS::Components::CAudioSource::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	using namespace VisionCore::Helpers;

	Serializer::DeserializeBoolean(p_doc, p_node, "autoplay", m_autoPlay);
	SetSpatial(Serializer::DeserializeBoolean(p_doc, p_node, "spatial"));
	SetVolume(Serializer::DeserializeFloat(p_doc, p_node, "volume"));
	SetPan(Serializer::DeserializeFloat(p_doc, p_node, "pan"));
	SetLooped(Serializer::DeserializeBoolean(p_doc, p_node, "looped"));
	SetPitch(Serializer::DeserializeFloat(p_doc, p_node, "pitch"));
	SetAttenuationThreshold(Serializer::DeserializeFloat(p_doc, p_node, "attenuation_threshold"));
	Serializer::DeserializeSound(p_doc, p_node, "audio_clip", m_sound);
}

void VisionCore::ECS::Components::CAudioSource::OnInspector(VisionUI::Internal::WidgetContainer& p_root)
{
	using namespace VisionAudio::Entities;
	using namespace VisionCore::Helpers;

	GUIDrawer::DrawSound(p_root, "Sound", m_sound);
	GUIDrawer::DrawBoolean(p_root, "Auto-play", m_autoPlay);
	GUIDrawer::DrawScalar<float>(p_root, "Volume", std::bind(&CAudioSource::GetVolume, this), std::bind(&CAudioSource::SetVolume, this, std::placeholders::_1), 0.01f, 0.0f, 1.0f);
	GUIDrawer::DrawScalar<float>(p_root, "Pan", std::bind(&CAudioSource::GetPan, this), std::bind(&CAudioSource::SetPan, this, std::placeholders::_1), 0.01f, -1.0f, 1.0f);
	GUIDrawer::DrawBoolean(p_root, "Looped", std::bind(&CAudioSource::IsLooped, this), std::bind(&CAudioSource::SetLooped, this, std::placeholders::_1));
	GUIDrawer::DrawScalar<float>(p_root, "Pitch", std::bind(&CAudioSource::GetPitch, this), std::bind(&CAudioSource::SetPitch, this, std::placeholders::_1), 0.01f, 0.01f, 10000.0f);
	GUIDrawer::DrawBoolean(p_root, "Spatial", std::bind(&CAudioSource::IsSpatial, this), std::bind(&CAudioSource::SetSpatial, this, std::placeholders::_1));
	GUIDrawer::DrawScalar<float>(p_root, "Attenuation threshold", std::bind(&CAudioSource::GetAttenuationThreshold, this), std::bind(&CAudioSource::SetAttenuationThreshold, this, std::placeholders::_1), 0.5f);

	GUIDrawer::CreateTitle(p_root, "Spatial graph");
	auto& graph = p_root.CreateWidget<VisionUI::Widgets::Plots::PlotLines>(std::vector<float>(), -0.1f, 1.1f);

	graph.size.y = 75.0f;

	auto& graphDispacther = graph.AddPlugin<VisionUI::Plugins::DataDispatcher<std::vector<float>>>();
	graphDispacther.RegisterGatherer([this, &graph]()->std::vector<float>
	{ 
		auto lerp = [](float a, float b, float t) { return a + t * (b - a); };

		std::vector<float> result;

		VisionMaths::FVector3 listenerPosition(0.0f, 0.0f, 0.0f);
		bool playMode = VisionCore::Global::ServiceLocator::Get<SceneSystem::SceneManager>().GetCurrentScene()->IsPlaying();
		auto listenerInfo = VisionCore::Global::ServiceLocator::Get<VisionAudio::Core::AudioEngine>().GetListenerInformation(!playMode);
		if (listenerInfo.has_value())
			listenerPosition = listenerInfo.value().first;

		float distanceToListener = VisionMaths::FVector3::Distance(listenerPosition, owner.transform.GetWorldPosition());

		for (float graphX = 0.0f; graphX < 50.0f; graphX += 0.25f)
		{
			float graphY = graphX < m_audioSource.GetAttenuationThreshold() ? 1.0f : 1.0f / (1.0f + 1.0f * (graphX - m_audioSource.GetAttenuationThreshold()));

			if (abs(graphX - distanceToListener) <= 0.25f)
			{
				graph.forceHover = static_cast<int>(graphX * 4.0f);
				graph.overlay = std::to_string(static_cast<int>(graphY * 100.0f)) + "%";
			}

			result.push_back(graphY);
		}

		return result;
	});
}

void VisionCore::ECS::Components::CAudioSource::OnEnable()
{
	if (m_autoPlay)
		Play();
}

void VisionCore::ECS::Components::CAudioSource::OnDisable()
{
	m_audioSource.Stop();
}
