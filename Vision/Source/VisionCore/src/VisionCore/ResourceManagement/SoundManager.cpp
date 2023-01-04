#include "VisionCore/ResourceManagement/SoundManager.h"

VisionAudio::Resources::Sound* VisionCore::ResourceManagement::SoundManager::CreateResource(const std::string& p_path)
{
	std::string realPath = GetRealPath(p_path);
	VisionAudio::Resources::Sound* sound = VisionAudio::Resources::Loaders::SoundLoader::Create(realPath);
	if (sound)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(sound) + offsetof(VisionAudio::Resources::Sound, path)) = p_path; // Force the resource path to fit the given path

	return sound;
}

void VisionCore::ResourceManagement::SoundManager::DestroyResource(VisionAudio::Resources::Sound* p_resource)
{
	VisionAudio::Resources::Loaders::SoundLoader::Destroy(p_resource);
}

void VisionCore::ResourceManagement::SoundManager::ReloadResource(VisionAudio::Resources::Sound* p_resource, const std::string& p_path)
{
}
