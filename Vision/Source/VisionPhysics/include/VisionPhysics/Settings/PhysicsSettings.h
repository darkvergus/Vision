#pragma once

#include <VisionMaths/FVector3.h>

namespace VisionPhysics::Settings
{
	/**
	* Data structure to give to the PhysicsEngine constructor to setup its settings
	*/
	struct PhysicsSettings
	{
		VisionMaths::FVector3 gravity = { 0.0f, -9.81f, 0.f };
	};
}