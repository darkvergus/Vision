#pragma once

#include "VisionEditor/Core/EditorActions.h"

namespace VisionEditor::Core
{
	template<typename T>
	inline VisionCore::ECS::Actor & EditorActions::CreateMonoComponentActor(bool p_focusOnCreation, VisionCore::ECS::Actor* p_parent)
	{
		auto& instance = CreateEmptyActor(false, p_parent);

		T& component = instance.AddComponent<T>();

        instance.SetName(component.GetName());

		if (p_focusOnCreation)
			SelectActor(instance);

		return instance;
	}
}