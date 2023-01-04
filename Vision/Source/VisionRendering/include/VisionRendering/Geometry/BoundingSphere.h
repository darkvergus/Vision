#pragma once

#include <VisionMaths/FVector3.h>

namespace VisionRendering::Geometry
{
	/**
	* Data structure that defines a bounding sphere (Position + radius)
	*/
	struct BoundingSphere
	{
		VisionMaths::FVector3 position;
		float radius;
	};
}