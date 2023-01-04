#include "VisionCore/ResourceManagement/ShaderManager.h"

VisionRendering::Resources::Shader* VisionCore::ResourceManagement::ShaderManager::CreateResource(const std::string & p_path)
{
	std::string realPath = GetRealPath(p_path);
	VisionRendering::Resources::Shader* shader = VisionRendering::Resources::Loaders::ShaderLoader::Create(realPath);
	if (shader)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(shader) + offsetof(VisionRendering::Resources::Shader, path)) = p_path; // Force the resource path to fit the given path

	return shader;
}

void VisionCore::ResourceManagement::ShaderManager::DestroyResource(VisionRendering::Resources::Shader* p_resource)
{
	VisionRendering::Resources::Loaders::ShaderLoader::Destroy(p_resource);
}

void VisionCore::ResourceManagement::ShaderManager::ReloadResource(VisionRendering::Resources::Shader* p_resource, const std::string& p_path)
{
	VisionRendering::Resources::Loaders::ShaderLoader::Recompile(*p_resource, p_path);
}
