#pragma once

#include "VisionGame/Core/Context.h"
#include "VisionGame/Core/Game.h"

namespace VisionGame::Core
{
	/**
	* Entry point of OvGame
	*/
	class Application
	{
	public:
		/**
		* Constructor
		*/
		Application();

		/**
		* Destructor
		*/
		~Application();

		/**
		* Run the app
		*/
		void Run();

		/**
		* Returns true if the app is running
		*/
		bool IsRunning() const;

	private:
		Context m_context;
		Game m_game;
	};
}