#pragma once

#include "VisionCore/ECS/Components/CLight.h"

namespace VisionCore::ECS { class Actor; }

namespace VisionCore::ECS::Components
{
	/**
	* A simple light that has no attenuation and that has a direction
	*/
	class CDirectionalLight : public CLight
	{
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		CDirectionalLight(ECS::Actor& p_owner);

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
	};
}