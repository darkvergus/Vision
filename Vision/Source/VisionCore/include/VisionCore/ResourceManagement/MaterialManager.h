#pragma once

#include "VisionCore/Resources/Loaders/MaterialLoader.h"
#include "VisionCore/ResourceManagement/AResourceManager.h"

namespace VisionCore::ResourceManagement
{
	/**
	* ResourceManager of materials
	*/
	class MaterialManager : public AResourceManager<VisionCore::Resources::Material>
	{
	public:
		/**
		* Create the resource identified by the given path
		* @param p_path
		*/
		virtual VisionCore::Resources::Material* CreateResource(const std::string & p_path) override;

		/**
		* Destroy the given resource
		* @param p_resource
		*/
		virtual void DestroyResource(VisionCore::Resources::Material* p_resource) override;

		/**
		* Reload the given resource
		* @param p_resource
		* @param p_path
		*/
		virtual void ReloadResource(VisionCore::Resources::Material* p_resource, const std::string& p_path) override;
	};
}