#pragma once

#include <VisionTools/Filesystem/tinyxml2.h>


#include "VisionCore/Helpers/Serializer.h"

namespace VisionCore::API
{
	/**
	* ISerializable is an interface for any class that can be serialized
	*/
	class ISerializable
	{
	public:
		/**
		* Called when the serialization is asked
		* @param p_doc
		* @param p_node
		*/
		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) = 0;

		/**
		* Called when the deserialization is asked
		* @param p_doc
		* @param p_node
		*/
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) = 0;

		/**
		* Default polymorphic destructor
		*/
		virtual ~ISerializable() = default;
	};
}