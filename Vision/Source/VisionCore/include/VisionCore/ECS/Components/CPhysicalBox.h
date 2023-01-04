#pragma once

#include "VisionCore/ECS/Components/AComponent.h"

#include "VisionCore/ECS/Components/CPhysicalObject.h"

namespace VisionCore::ECS { class Actor; }

namespace VisionCore::ECS::Components
{
	/**
	* Represent a physical object with a box shape
	*/
	class CPhysicalBox : public CPhysicalObject
	{
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		CPhysicalBox(ECS::Actor& p_owner);

		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

		/**
		* Defines the size of the box shape
		* @param p_size
		*/
		void SetSize(const VisionMaths::FVector3& p_size);

		/**
		* Returns the size of the box shape
		*/
		VisionMaths::FVector3 GetSize() const;

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