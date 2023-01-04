#pragma once

#include <VisionCore/ResourceManagement/TextureManager.h>
#include <VisionCore/ResourceManagement/ModelManager.h>
#include <VisionCore/ResourceManagement/ShaderManager.h>

namespace VisionEditor::Core
{
	/**
	* Handle the creation and storage of editor specific resources
	*/
	class EditorResources
	{
	public:
		/**
		* Constructor
		* @param p_editorAssetsPath
		*/
		EditorResources(const std::string& p_editorAssetsPath);

		/**
		* Destructor
		*/
		~EditorResources();

		/**
		* Returns the file icon identified by the given string or nullptr on fail
		* @param p_filename
		*/
		VisionRendering::Resources::Texture* GetFileIcon(const std::string& p_filename);

		/**
		* Returns the texture identified by the given string or nullptr on fail
		* @param p_id
		*/
		VisionRendering::Resources::Texture* GetTexture(const std::string& p_id);

		/**
		* Returns the model identified by the given string or nullptr on fail
		* @param p_id
		*/
		VisionRendering::Resources::Model* GetModel(const std::string& p_id);

		/**
		* Returns the shader identified by the given string or nullptr on fail
		* @param p_id
		*/
		VisionRendering::Resources::Shader* GetShader(const std::string& p_id);

	private:
		std::unordered_map<std::string, VisionRendering::Resources::Texture*> m_textures;
		std::unordered_map<std::string, VisionRendering::Resources::Model*> m_models;
		std::unordered_map<std::string, VisionRendering::Resources::Shader*> m_shaders;
	};
}