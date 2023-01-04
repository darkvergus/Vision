#pragma once

#include <VisionUI/Internal/WidgetContainer.h>

#include "VisionCore/API/ISerializable.h"
#include "VisionCore/Helpers/GUIDrawer.h"

namespace VisionCore::API
{
	/**
	* IInspectorItem is an interface to any class that is serializable and drawable on inspector
	*/
	class IInspectorItem : public ISerializable
	{
	public:
		/**
		* Called when the inspector needs to create the UI elements for the given IInspectorItem
		* @param p_root
		*/
		virtual void OnInspector(VisionUI::Internal::WidgetContainer& p_root) = 0;
	};
}