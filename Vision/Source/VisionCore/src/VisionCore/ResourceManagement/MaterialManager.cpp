#include "VisionCore/ResourceManagement/MaterialManager.h"

VisionCore::Resources::Material * VisionCore::ResourceManagement::MaterialManager::CreateResource(const std::string & p_path)
{
	std::string realPath = GetRealPath(p_path);

	Resources::Material* material = VisionCore::Resources::Loaders::MaterialLoader::Create(realPath);
	if (material)
	{
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(material) + offsetof(Resources::Material, path)) = p_path; // Force the resource path to fit the given path
	}

	return material;
}

void VisionCore::ResourceManagement::MaterialManager::DestroyResource(VisionCore::Resources::Material * p_resource)
{
	VisionCore::Resources::Loaders::MaterialLoader::Destroy(p_resource);
}

void VisionCore::ResourceManagement::MaterialManager::ReloadResource(VisionCore::Resources::Material* p_resource, const std::string& p_path)
{
	VisionCore::Resources::Loaders::MaterialLoader::Reload(*p_resource, p_path);
}
