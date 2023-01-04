#include "VisionCore/ECS/PhysicsWrapper.h"

#include "VisionCore/Global/ServiceLocator.h"

#include <VisionPhysics/Core/PhysicsEngine.h>

std::optional<VisionCore::ECS::PhysicsWrapper::RaycastHit> VisionCore::ECS::PhysicsWrapper::Raycast(VisionMaths::FVector3 p_origin, VisionMaths::FVector3 p_direction, float p_distance)
{
	if (auto result = SERVICE(VisionPhysics::Core::PhysicsEngine).Raycast(p_origin, p_direction, p_distance))
	{
		RaycastHit finalResult;

		finalResult.FirstResultObject = std::addressof(result.value().FirstResultObject->GetUserData<std::reference_wrapper<Components::CPhysicalObject>>().get());
		for (auto object : result.value().ResultObjects)
			finalResult.ResultObjects.push_back(std::addressof(object->GetUserData<std::reference_wrapper<Components::CPhysicalObject>>().get()));

		return finalResult;
	}
	else
		return {};
}
