#pragma once

#include <VisionAudio/Resources/Loaders/SoundLoader.h>

#include "VisionCore/ResourceManagement/AResourceManager.h"

namespace VisionCore::ResourceManagement
{
	/**
	* ResourceManager of sounds
	*/
	class SoundManager : public AResourceManager<VisionAudio::Resources::Sound>
	{
	public:
		/**
		* Create the resource identified by the given path
		* @param p_path
		*/
		virtual VisionAudio::Resources::Sound* CreateResource(const std::string& p_path) override;

		/**
		* Destroy the given resource
		* @param p_resource
		*/
		virtual void DestroyResource(VisionAudio::Resources::Sound* p_resource) override;

		/**
		* Reload the given resource
		* @param p_resource
		* @param p_path
		*/
		virtual void ReloadResource(VisionAudio::Resources::Sound* p_resource, const std::string& p_path) override;
	};
}