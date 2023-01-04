#pragma once

#include <bullet/btBulletCollisionCommon.h>

#include <vector>


#include "VisionPhysics/Entities/PhysicalObject.h"

namespace VisionPhysics::Entities
{
	/**
	* Data structure that holds raycast hit information
	*/
	struct RaycastHit
	{
		Entities::PhysicalObject* FirstResultObject = nullptr;
		std::vector<Entities::PhysicalObject*> ResultObjects;
	};
}