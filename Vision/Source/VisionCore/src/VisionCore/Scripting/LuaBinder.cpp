#include "VisionCore/Scripting/LuaBinder.h"

#include "VisionCore/Scripting/LuaMathsBinder.h"
#include "VisionCore/Scripting/LuaActorBinder.h"
#include "VisionCore/Scripting/LuaComponentBinder.h"
#include "VisionCore/Scripting/LuaGlobalsBinder.h"

void VisionCore::Scripting::LuaBinder::CallBinders(sol::state& p_luaState)
{
	auto& L = p_luaState;

	LuaMathsBinder::BindMaths(L);
	LuaActorBinder::BindActor(L);
	LuaComponentBinder::BindComponent(L);
	LuaGlobalsBinder::BindGlobals(L);
}