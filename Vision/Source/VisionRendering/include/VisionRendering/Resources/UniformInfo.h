#pragma once

#include <string>
#include <any>

#include "VisionRendering/Resources/UniformType.h"

namespace VisionRendering::Resources
{
	/**
	* Data structure containing information about a uniform
	*/
	struct UniformInfo
	{
		UniformType		type;
		std::string		name;
		uint32_t		location;
		std::any		defaultValue;
	};
}