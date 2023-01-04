#pragma once

#include <VisionRendering/Buffers/UniformBuffer.h>
#include <VisionRendering/Buffers/ShaderStorageBuffer.h>

#include <VisionPhysics/Core/PhysicsEngine.h>

#include <VisionWindowing/Context/Device.h>
#include <VisionWindowing/Inputs/InputManager.h>
#include <VisionWindowing/Window.h>

#include <VisionUI/Core/UIManager.h>

#include <VisionCore/ECS/Renderer.h>
#include <VisionCore/ResourceManagement/ModelManager.h>
#include <VisionCore/ResourceManagement/TextureManager.h>
#include <VisionCore/ResourceManagement/ShaderManager.h>
#include <VisionCore/ResourceManagement/MaterialManager.h>
#include <VisionCore/ResourceManagement/SoundManager.h>
#include <VisionCore/SceneSystem/SceneManager.h>
#include <VisionCore/Scripting/ScriptInterpreter.h>

#include <VisionAudio/Core/AudioEngine.h>
#include <VisionAudio/Core/AudioPlayer.h>

#include <VisionTools/Filesystem/IniFile.h>

namespace VisionGame::Core
{
	/**
	* The Context handle the engine features setup
	*/
	class Context
	{
	public:
		/**
		* Constructor
		*/
		Context();

		/**
		* Destructor
		*/
		~Context();

	public:
		const std::string engineAssetsPath;
		const std::string projectAssetsPath;
		const std::string projectScriptsPath;

		std::unique_ptr<VisionWindowing::Context::Device>				device;
		std::unique_ptr<VisionWindowing::Window>						window;
		std::unique_ptr<VisionWindowing::Inputs::InputManager>			inputManager;
		std::unique_ptr<VisionRendering::Context::Driver>				driver;
		std::unique_ptr<VisionCore::ECS::Renderer>						renderer;
		std::unique_ptr<VisionUI::Core::UIManager>						uiManager;
		std::unique_ptr<VisionPhysics::Core::PhysicsEngine>				physicsEngine;
		std::unique_ptr<VisionAudio::Core::AudioEngine>					audioEngine;
		std::unique_ptr<VisionAudio::Core::AudioPlayer>					audioPlayer;
		std::unique_ptr<VisionCore::Scripting::ScriptInterpreter>		scriptInterpreter;
		std::unique_ptr<VisionRendering::Buffers::UniformBuffer>		engineUBO;
		std::unique_ptr<VisionRendering::Buffers::ShaderStorageBuffer>	lightSSBO;

		VisionCore::SceneSystem::SceneManager sceneManager;

		VisionCore::ResourceManagement::ModelManager	modelManager;
		VisionCore::ResourceManagement::TextureManager	textureManager;
		VisionCore::ResourceManagement::ShaderManager	shaderManager;
		VisionCore::ResourceManagement::MaterialManager	materialManager;
		VisionCore::ResourceManagement::SoundManager	soundManager;
		
		VisionTools::Filesystem::IniFile projectSettings;
	};
}