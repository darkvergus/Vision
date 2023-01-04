#pragma once

#include <VisionRendering/Resources/Loaders/ShaderLoader.h>

#include "VisionCore/ResourceManagement/AResourceManager.h"

namespace VisionCore::ResourceManagement
{
	/**
	* ResourceManager of shaders
	*/
	class ShaderManager : public AResourceManager<VisionRendering::Resources::Shader>
	{
	public:
		/**
		* Create the resource identified by the given path
		* @param p_path
		*/
		virtual VisionRendering::Resources::Shader* CreateResource(const std::string & p_path) override;

		/**
		* Destroy the given resource
		* @param p_resource
		*/
		virtual void DestroyResource(VisionRendering::Resources::Shader* p_resource) override;

		/**
		* Reload the given resource
		* @param p_resource
		* @param p_path
		*/
		virtual void ReloadResource(VisionRendering::Resources::Shader* p_resource, const std::string& p_path) override;
	};
}