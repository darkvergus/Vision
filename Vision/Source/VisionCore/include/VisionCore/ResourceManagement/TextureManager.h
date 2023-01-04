#pragma once

#include <VisionRendering/Resources/Loaders/TextureLoader.h>

#include "VisionCore/ResourceManagement/AResourceManager.h"

namespace VisionCore::ResourceManagement
{
	/**
	* ResourceManager of textures
	*/
	class TextureManager : public AResourceManager<VisionRendering::Resources::Texture>
	{
	public:
		/**
		* Create the resource identified by the given path
		* @param p_path
		*/
		virtual VisionRendering::Resources::Texture* CreateResource(const std::string & p_path) override;

		/**
		* Destroy the given resource
		* @param p_resource
		*/
		virtual void DestroyResource(VisionRendering::Resources::Texture* p_resource) override;

		/**
		* Reload the given resource
		* @param p_resource
		* @param p_path
		*/
		virtual void ReloadResource(VisionRendering::Resources::Texture* p_resource, const std::string& p_path) override;
	};
}