#pragma once

#include <VisionUI/Modules/Canvas.h>
#include <VisionCore/ECS/Components/CCamera.h>

#include "VisionGame/Core/Context.h"
#include "VisionGame/Core/GameRenderer.h"

#include "VisionGame/Utils/FPSCounter.h"

#ifdef _DEBUG
#include "VisionGame/Debug/DriverInfo.h"
#include "VisionGame/Debug/GameProfiler.h"
#include "VisionGame/Debug/FrameInfo.h"
#endif

namespace VisionGame::Core
{
	/**
	* Handle the game logic
	*/
	class Game
	{
	public:
		/**
		* Create the game
		* @param p_context
		*/
		Game(Context& p_context);

		/**
		* Destroy the game
		*/
		~Game();

		/**
		* Pre-update of the game logic
		*/
		void PreUpdate();

		/**
		* Update the game logic
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);

		/**
		* Post-update of the game logic
		*/
		void PostUpdate();

	private:
		float m_elapsed = 0.0f;

		VisionGame::Core::Context& m_context;
		VisionUI::Modules::Canvas m_canvas;

		VisionGame::Core::GameRenderer m_gameRenderer;

		/* Debug elements */
		VisionGame::Utils::FPSCounter	m_fpsCounter;

		#ifdef _DEBUG
		VisionGame::Debug::DriverInfo	m_driverInfo;
		VisionGame::Debug::GameProfiler m_gameProfiler;
		VisionGame::Debug::FrameInfo	m_frameInfo;
		#endif

		#ifdef _DEBUG
		bool m_showDebugInformation = true;
		#else
		bool m_showDebugInformation = false;
		#endif
	};
}