#include "VisionCore/ResourceManagement/TextureManager.h"
#include "VisionRendering/Settings/DriverSettings.h"

#include <VisionTools/Filesystem/IniFile.h>

std::tuple<VisionRendering::Settings::ETextureFilteringMode, VisionRendering::Settings::ETextureFilteringMode, bool> GetAssetMetadata(const std::string& p_path)
{
	auto metaFile = VisionTools::Filesystem::IniFile(p_path + ".meta");

	auto min = metaFile.GetOrDefault("MIN_FILTER", static_cast<int>(VisionRendering::Settings::ETextureFilteringMode::LINEAR_MIPMAP_LINEAR));
	auto mag = metaFile.GetOrDefault("MAG_FILTER", static_cast<int>(VisionRendering::Settings::ETextureFilteringMode::LINEAR));
	auto mipmap = metaFile.GetOrDefault("ENABLE_MIPMAPPING", true);

	return { static_cast<VisionRendering::Settings::ETextureFilteringMode>(min), static_cast<VisionRendering::Settings::ETextureFilteringMode>(mag), mipmap };
}

VisionRendering::Resources::Texture* VisionCore::ResourceManagement::TextureManager::CreateResource(const std::string & p_path)
{
	std::string realPath = GetRealPath(p_path);

	auto [min, mag, mipmap] = GetAssetMetadata(realPath);

	VisionRendering::Resources::Texture* texture = VisionRendering::Resources::Loaders::TextureLoader::Create(realPath, min, mag, mipmap);
	if (texture)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(texture) + offsetof(VisionRendering::Resources::Texture, path)) = p_path; // Force the resource path to fit the given path

	return texture;
}

void VisionCore::ResourceManagement::TextureManager::DestroyResource(VisionRendering::Resources::Texture* p_resource)
{
	VisionRendering::Resources::Loaders::TextureLoader::Destroy(p_resource);
}

void VisionCore::ResourceManagement::TextureManager::ReloadResource(VisionRendering::Resources::Texture* p_resource, const std::string& p_path)
{
	std::string realPath = GetRealPath(p_path);

	auto [min, mag, mipmap] = GetAssetMetadata(realPath);

	VisionRendering::Resources::Loaders::TextureLoader::Reload(*p_resource, realPath, min, mag, mipmap);
}
