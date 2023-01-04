#pragma once

#include <sol.hpp>

namespace VisionCore::Scripting
{
	/**
	* Handle lua components binding
	*/
	class LuaComponentBinder
	{
	public:
		/**
		* Bind engine component-related features to lua
		*/
		static void BindComponent(sol::state& p_luaState);
	};
}