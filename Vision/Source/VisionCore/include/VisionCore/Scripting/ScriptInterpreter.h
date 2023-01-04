#pragma once

#include <vector>

#include <sol.hpp>

#include "VisionCore/ECS/Components/Behaviour.h"

namespace VisionCore::Scripting
{
	/**
	* Handles script interpretation
	*/
	class ScriptInterpreter
	{
	public:
		/**
		* Constructor
		* @param p_scriptRootFolder
		*/
		ScriptInterpreter(const std::string& p_scriptRootFolder);

		/**
		* Destructor
		*/
		~ScriptInterpreter();

		/**
		* Handle the creation of the lua context and bind engine functions to lua
		*/
		void CreateLuaContextAndBindGlobals();

		/**
		* Destroy the lua context
		*/
		void DestroyLuaContext();

		/**
		* Consider a behaviour
		* @param p_toConsider
		*/
		void Consider(VisionCore::ECS::Components::Behaviour* p_toConsider);

		/**
		* Unconsider a behaviour
		* @param p_toUnconsider
		*/
		void Unconsider(VisionCore::ECS::Components::Behaviour* p_toUnconsider);

		/**
		* Refresh every scripts
		*/
		void RefreshAll();

		/**
		* Returns true if every scripts are OK
		*/
		bool IsOk() const;

	private:
		std::unique_ptr<sol::state> m_luaState;
		std::string m_scriptRootFolder;
		std::vector<VisionCore::ECS::Components::Behaviour*> m_behaviours;
		bool m_isOk;
	};
}