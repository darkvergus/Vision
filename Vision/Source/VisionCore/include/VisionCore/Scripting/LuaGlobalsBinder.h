#pragma once

#include <sol.hpp>

namespace VisionCore::Scripting
{
	/**
	* Handle lua global binding
	*/
	class LuaGlobalsBinder
	{
	public:
		/**
		* Bind engine global features to lua
		*/
		static void BindGlobals(sol::state& p_luaState);
	};
}