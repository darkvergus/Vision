#pragma once

#include <VisionRendering/Resources/Loaders/ModelLoader.h>

#include "VisionCore/ResourceManagement/AResourceManager.h"

namespace VisionCore::ResourceManagement
{
	/**
	* ResourceManager of models
	*/
	class ModelManager : public AResourceManager<VisionRendering::Resources::Model>
	{
	public:
		/**
		* Create the resource identified by the given path
		* @param p_path
		*/
		virtual VisionRendering::Resources::Model* CreateResource(const std::string & p_path) override;

		/**
		* Destroy the given resource
		* @param p_resource
		*/
		virtual void DestroyResource(VisionRendering::Resources::Model* p_resource) override;

		/**
		* Reload the given resource
		* @param p_resource
		* @param p_path
		*/
		virtual void ReloadResource(VisionRendering::Resources::Model* p_resource, const std::string& p_path) override;
	};
}