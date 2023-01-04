#pragma once

#include <VisionTools/Filesystem/IniFile.h>

#include <VisionRendering/Buffers/UniformBuffer.h>
#include <VisionRendering/Buffers/ShaderStorageBuffer.h>
#include <VisionRendering/Core/ShapeDrawer.h>

#include <VisionPhysics/Core/PhysicsEngine.h>

#include <VisionAudio/Core/AudioEngine.h>

#include <VisionWindowing/Context/Device.h>
#include <VisionWindowing/Inputs/InputManager.h>
#include <VisionWindowing/Window.h>

#include <VisionCore/ECS/Renderer.h>
#include <VisionCore/ResourceManagement/ModelManager.h>
#include <VisionCore/ResourceManagement/TextureManager.h>
#include <VisionCore/ResourceManagement/ShaderManager.h>
#include <VisionCore/ResourceManagement/MaterialManager.h>
#include <VisionCore/ResourceManagement/SoundManager.h>
#include <VisionCore/SceneSystem/SceneManager.h>
#include <VisionCore/Scripting/ScriptInterpreter.h>

#include <VisionUI/Core/UIManager.h>
#include <VisionAudio/Core/AudioEngine.h>
#include <VisionAudio/Core/AudioPlayer.h>

#include "VisionEditor/Core/EditorResources.h"

namespace VisionEditor::Core
{
	/**
	* The Context handle the engine features setup
	*/
	class Context
	{
	public:
		/**
		* Constructor
		* @param p_projectPath
		* @param p_projectName
		*/
		Context(const std::string& p_projectPath, const std::string& p_projectName);

		/**
		* Destructor
		*/
		~Context();

		/**
		* Reset project settings ini file
		*/
		void ResetProjectSettings();

		/**
		* Verify that project settings are complete (No missing key).
		* Returns true if the integrity is verified
		*/
		bool IsProjectSettingsIntegrityVerified();

		/**
		* Apply project settings to the ini file
		*/
		void ApplyProjectSettings();

	public:
		const std::string projectPath;
		const std::string projectName;
		const std::string projectFilePath;
		const std::string engineAssetsPath;
		const std::string projectAssetsPath;
		const std::string projectScriptsPath;
		const std::string editorAssetsPath;

		std::unique_ptr<VisionWindowing::Context::Device>			device;
		std::unique_ptr<VisionWindowing::Window>					window;
		std::unique_ptr<VisionWindowing::Inputs::InputManager>		inputManager;
		std::unique_ptr<VisionRendering::Context::Driver>			driver;
		std::unique_ptr<VisionCore::ECS::Renderer>					renderer;
		std::unique_ptr<VisionRendering::Core::ShapeDrawer>			shapeDrawer;
		std::unique_ptr<VisionUI::Core::UIManager>					uiManager;
		std::unique_ptr<VisionPhysics::Core::PhysicsEngine>			physicsEngine;
		std::unique_ptr<VisionAudio::Core::AudioEngine>				audioEngine;
		std::unique_ptr<VisionAudio::Core::AudioPlayer>				audioPlayer;
		std::unique_ptr<VisionEditor::Core::EditorResources>		editorResources;

		std::unique_ptr<VisionCore::Scripting::ScriptInterpreter>	scriptInterpreter;
		std::unique_ptr<VisionRendering::Buffers::UniformBuffer>	engineUBO;

		std::unique_ptr<VisionRendering::Buffers::ShaderStorageBuffer>	lightSSBO;
		std::unique_ptr<VisionRendering::Buffers::ShaderStorageBuffer>	simulatedLightSSBO;
		
		VisionCore::SceneSystem::SceneManager sceneManager;

		VisionCore::ResourceManagement::ModelManager	modelManager;
		VisionCore::ResourceManagement::TextureManager	textureManager;
		VisionCore::ResourceManagement::ShaderManager	shaderManager;
		VisionCore::ResourceManagement::MaterialManager	materialManager;
		VisionCore::ResourceManagement::SoundManager	soundManager;

		VisionWindowing::Settings::WindowSettings windowSettings;

		VisionTools::Filesystem::IniFile projectSettings;
	};
}
