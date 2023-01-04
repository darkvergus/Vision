#include <VisionDebug/Logger.h>

#include "VisionCore/Scripting/LuaBinder.h"
#include "VisionCore/Scripting/ScriptInterpreter.h"

VisionCore::Scripting::ScriptInterpreter::ScriptInterpreter(const std::string& p_scriptRootFolder) :
	m_scriptRootFolder(p_scriptRootFolder)
{
	CreateLuaContextAndBindGlobals();

	/* Listen to behaviours */
	VisionCore::ECS::Components::Behaviour::CreatedEvent	+= std::bind(&ScriptInterpreter::Consider, this, std::placeholders::_1);
	VisionCore::ECS::Components::Behaviour::DestroyedEvent	+= std::bind(&ScriptInterpreter::Unconsider, this, std::placeholders::_1);
}

VisionCore::Scripting::ScriptInterpreter::~ScriptInterpreter()
{
	DestroyLuaContext();
}

void VisionCore::Scripting::ScriptInterpreter::CreateLuaContextAndBindGlobals()
{
	if (!m_luaState)
	{
		m_luaState = std::make_unique<sol::state>();
		m_luaState->open_libraries(sol::lib::base, sol::lib::math);
		VisionCore::Scripting::LuaBinder::CallBinders(*m_luaState);
		m_isOk = true;

		std::for_each(m_behaviours.begin(), m_behaviours.end(), [this](ECS::Components::Behaviour* behaviour)
		{
			if (!behaviour->RegisterToLuaContext(*m_luaState, m_scriptRootFolder))
				m_isOk = false;
		});

		if (!m_isOk)
			LOG_ERROR("Script interpreter failed to register scripts. Check your lua scripts");
	}
}

void VisionCore::Scripting::ScriptInterpreter::DestroyLuaContext()
{
	if (m_luaState)
	{
		std::for_each(m_behaviours.begin(), m_behaviours.end(), [this](ECS::Components::Behaviour* behaviour)
		{
			behaviour->UnregisterFromLuaContext();
		});

		m_luaState.reset();
		m_isOk = false;
	}
}

void VisionCore::Scripting::ScriptInterpreter::Consider(VisionCore::ECS::Components::Behaviour* p_toConsider)
{
	if (m_luaState)
	{
		m_behaviours.push_back(p_toConsider);

		if (!p_toConsider->RegisterToLuaContext(*m_luaState, m_scriptRootFolder))
			m_isOk = false;
	}
}

void VisionCore::Scripting::ScriptInterpreter::Unconsider(VisionCore::ECS::Components::Behaviour* p_toUnconsider)
{
	if (m_luaState)
		p_toUnconsider->UnregisterFromLuaContext();

	m_behaviours.erase(std::remove_if(m_behaviours.begin(), m_behaviours.end(), [p_toUnconsider](ECS::Components::Behaviour* behaviour)
	{
		return p_toUnconsider == behaviour;
	}));

	RefreshAll(); // Unconsidering a script is impossible with Lua, we have to reparse every behaviours
}

void VisionCore::Scripting::ScriptInterpreter::RefreshAll()
{
	DestroyLuaContext();
	CreateLuaContextAndBindGlobals();
}

bool VisionCore::Scripting::ScriptInterpreter::IsOk() const
{
	return m_isOk;
}
