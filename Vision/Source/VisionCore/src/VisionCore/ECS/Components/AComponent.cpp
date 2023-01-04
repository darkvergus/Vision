#include "VisionCore/ECS/Components/AComponent.h"
#include "VisionCore/ECS/Actor.h"

VisionCore::ECS::Components::AComponent::AComponent(ECS::Actor& p_owner) : owner(p_owner)
{
}

VisionCore::ECS::Components::AComponent::~AComponent()
{
	if (owner.IsActive())
	{
		OnDisable();
		OnDestroy();
	}
}
