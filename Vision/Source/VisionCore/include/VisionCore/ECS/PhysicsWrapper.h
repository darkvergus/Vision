#pragma once

#include <optional>
#include <vector>

#include <VisionPhysics/Entities/RaycastHit.h>

#include <VisionMaths/FVector3.h>

#include "VisionCore/ECS/Components/CPhysicalObject.h"

namespace VisionCore::ECS
{
	/**
	* Simple class that contains wrappers for OvPhysics in an ECS style
	*/
	class PhysicsWrapper
	{
	public:
		/**
		* Simple data structure that wraps the physics RaycastHit with physics components
		*/
		struct RaycastHit
		{
			Components::CPhysicalObject* FirstResultObject = nullptr;
			std::vector<Components::CPhysicalObject*> ResultObjects;
		};

		/* Casts a ray against all Physical Object in the Scene and returns information on what was hit
		 * @param p_origin
		 * @param p_end
		 */
		static std::optional<RaycastHit> Raycast(VisionMaths::FVector3 p_origin, VisionMaths::FVector3 p_direction, float p_distance);
	};
}