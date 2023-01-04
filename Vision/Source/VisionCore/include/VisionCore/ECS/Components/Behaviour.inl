#pragma once


#include <VisionDebug/Logger.h>

#include "VisionCore/ECS/Components/Behaviour.h"

namespace VisionCore::ECS::Components
{
	template<typename ...Args>
	inline void Components::Behaviour::LuaCall(const std::string& p_functionName, Args&& ...p_args)
	{
		if (m_object.valid())
		{
			if (m_object[p_functionName].valid())
			{
				sol::protected_function pfr = m_object[p_functionName];
				auto pfrResult = pfr.call(m_object, std::forward<Args>(p_args)...);
				if (!pfrResult.valid())
				{
					sol::error err = pfrResult;
					LOG_ERROR(err.what());
				}
			}
		}
	}
}