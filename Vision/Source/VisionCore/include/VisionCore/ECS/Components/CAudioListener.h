#pragma once

#include "AComponent.h"

#include <VisionTools/Eventing/Event.h>

#include <VisionAudio/Entities/AudioListener.h>

namespace VisionCore::ECS { class Actor; }

namespace VisionCore::ECS::Components
{
	/**
	* Represents an audio listener. The distance between audio sources and audio listener will affect the sound
	* attenuation of spatialized sounds
	*/
	class CAudioListener : public AComponent
	{
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		CAudioListener(ECS::Actor& p_owner);

		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

		/**
		* Serialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		/**
		* Deserialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		/**
		* Defines how the component should be drawn in the inspector
		* @param p_root
		*/
		virtual void OnInspector(VisionUI::Internal::WidgetContainer& p_root) override;

	private:
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		VisionAudio::Entities::AudioListener m_audioListener;
	};
}