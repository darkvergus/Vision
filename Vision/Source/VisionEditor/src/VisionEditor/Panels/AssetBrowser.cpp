#include <fstream>
#include <iostream>

#include <VisionUI/Widgets/Texts/TextClickable.h>
#include <VisionUI/Widgets/Visual/Image.h>
#include <VisionUI/Widgets/Visual/Separator.h>
#include <VisionUI/Widgets/Buttons/Button.h>
#include <VisionUI/Widgets/Layout/Group.h>
#include <VisionUI/Plugins/DDSource.h>
#include <VisionUI/Plugins/DDTarget.h>
#include <VisionUI/Plugins/ContextualMenu.h>

#include <VisionWindowing/Dialogs/MessageBox.h>
#include <VisionWindowing/Dialogs/SaveFileDialog.h>
#include <VisionWindowing/Dialogs/OpenFileDialog.h>
#include <VisionTools/Utils/SystemCalls.h>
#include <VisionTools/Utils/PathParser.h>
#include <VisionTools/Utils/String.h>

#include <VisionCore/Global/ServiceLocator.h>
#include <VisionCore/ResourceManagement/ModelManager.h>
#include <VisionCore/ResourceManagement/TextureManager.h>
#include <VisionCore/ResourceManagement/ShaderManager.h>

#include <VisionDebug/Logger.h>

#include "VisionEditor/Panels/AssetBrowser.h"
#include "VisionEditor/Panels/AssetView.h"
#include "VisionEditor/Panels/MaterialEditor.h"
#include "VisionEditor/Panels/AssetProperties.h"
#include "VisionEditor/Core/EditorActions.h"
#include "VisionEditor/Core/EditorResources.h"

using namespace VisionUI::Panels;
using namespace VisionUI::Widgets;

#define FILENAMES_CHARS VisionEditor::Panels::AssetBrowser::__FILENAMES_CHARS

const std::string FILENAMES_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-_=+ 0123456789()[]";

std::string GetAssociatedMetaFile(const std::string& p_assetPath)
{
	return p_assetPath + ".meta";
}

void RenameAsset(const std::string& p_prev, const std::string& p_new)
{
	std::filesystem::rename(p_prev, p_new);

	if (const std::string previousMetaPath = GetAssociatedMetaFile(p_prev); std::filesystem::exists(previousMetaPath))
	{
		if (const std::string newMetaPath = GetAssociatedMetaFile(p_new); !std::filesystem::exists(newMetaPath))
		{
			std::filesystem::rename(previousMetaPath, newMetaPath);
		}
		else
		{
			LOG_ERROR(newMetaPath + " is already existing, .meta creation failed");
		}
	}
}

void RemoveAsset(const std::string& p_toDelete)
{
	std::filesystem::remove(p_toDelete);

	if (const std::string metaPath = GetAssociatedMetaFile(p_toDelete); std::filesystem::exists(metaPath))
	{
		std::filesystem::remove(metaPath);
	}
}

class TexturePreview : public VisionUI::Plugins::IPlugin
{
public:
	TexturePreview() : image(0, { 80, 80 })
	{

	}

	void SetPath(const std::string& p_path)
	{
		texture = VisionCore::Global::ServiceLocator::Get<VisionCore::ResourceManagement::TextureManager>()[p_path];
	}

	virtual void Execute() override
	{
		if (ImGui::IsItemHovered())
		{
			if (texture)
				image.textureID.id = texture->id;

			ImGui::BeginTooltip();
			image.Draw();
			ImGui::EndTooltip();
		}
	}

	VisionRendering::Resources::Texture* texture = nullptr;
	VisionUI::Widgets::Visual::Image image;
};

class BrowserItemContextualMenu : public VisionUI::Plugins::ContextualMenu
{
public:
	BrowserItemContextualMenu(const std::string p_filePath, bool p_protected = false) : m_protected(p_protected), filePath(p_filePath) {}

	virtual void CreateList()
	{
		if (!m_protected)
		{
			auto& renameMenu = CreateWidget<VisionUI::Widgets::Menu::MenuList>("Rename to...");
			auto& deleteAction = CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Delete");

			auto& nameEditor = renameMenu.CreateWidget<VisionUI::Widgets::InputFields::InputText>("");
			nameEditor.selectAllOnClick = true;

			renameMenu.ClickedEvent += [this, &nameEditor]
			{
				nameEditor.content = VisionTools::Utils::PathParser::GetElementName(filePath);

				if (!std::filesystem::is_directory(filePath))
					if (size_t pos = nameEditor.content.rfind('.'); pos != std::string::npos)
						nameEditor.content = nameEditor.content.substr(0, pos);
			};

			deleteAction.ClickedEvent += [this] { DeleteItem(); };

			nameEditor.EnterPressedEvent += [this](std::string p_newName)
			{
				if (!std::filesystem::is_directory(filePath))
					p_newName += '.' + VisionTools::Utils::PathParser::GetExtension(filePath);

				/* Clean the name (Remove special chars) */
				p_newName.erase(std::remove_if(p_newName.begin(), p_newName.end(), [](auto& c)
				{
					return std::find(FILENAMES_CHARS.begin(), FILENAMES_CHARS.end(), c) == FILENAMES_CHARS.end();
				}), p_newName.end());

				std::string containingFolderPath = VisionTools::Utils::PathParser::GetContainingFolder(filePath);
				std::string newPath = containingFolderPath + p_newName;
				std::string oldPath = filePath;

				if (filePath != newPath && !std::filesystem::exists(newPath))
					filePath = newPath;

				if (std::filesystem::is_directory(oldPath))
					filePath += '\\';

				RenamedEvent.Invoke(oldPath, newPath);
			};
		}
	}

	virtual void Execute() override
	{
		if (m_widgets.size() > 0)
			VisionUI::Plugins::ContextualMenu::Execute();
	}

	virtual void DeleteItem() = 0;

public:
	bool m_protected;
	std::string filePath;
	VisionTools::Eventing::Event<std::string> DestroyedEvent;
	VisionTools::Eventing::Event<std::string, std::string> RenamedEvent;
};

class FolderContextualMenu : public BrowserItemContextualMenu
{
public:
	FolderContextualMenu(const std::string& p_filePath, bool p_protected = false) : BrowserItemContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& showInExplorer = CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Show in explorer");
		showInExplorer.ClickedEvent += [this]
		{
			VisionTools::Utils::SystemCalls::ShowInExplorer(filePath);
		};

		if (!m_protected)
		{
			auto& importAssetHere = CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Import Here...");
			importAssetHere.ClickedEvent += [this]
			{
				if (EDITOR_EXEC(ImportAssetAtLocation(filePath)))
				{
					VisionUI::Widgets::Layout::TreeNode* pluginOwner = reinterpret_cast<VisionUI::Widgets::Layout::TreeNode*>(userData);
					pluginOwner->Close();
					EDITOR_EXEC(DelayAction(std::bind(&VisionUI::Widgets::Layout::TreeNode::Open, pluginOwner)));
				}
			};

			auto& createMenu = CreateWidget<VisionUI::Widgets::Menu::MenuList>("Create..");

			auto& createFolderMenu = createMenu.CreateWidget<VisionUI::Widgets::Menu::MenuList>("Folder");
			auto& createSceneMenu = createMenu.CreateWidget<VisionUI::Widgets::Menu::MenuList>("Scene");
			auto& createShaderMenu = createMenu.CreateWidget<VisionUI::Widgets::Menu::MenuList>("Shader");
			auto& createMaterialMenu = createMenu.CreateWidget<VisionUI::Widgets::Menu::MenuList>("Material");

			auto& createStandardShaderMenu = createShaderMenu.CreateWidget<VisionUI::Widgets::Menu::MenuList>("Standard template");
			auto& createStandardPBRShaderMenu = createShaderMenu.CreateWidget<VisionUI::Widgets::Menu::MenuList>("Standard PBR template");
			auto& createUnlitShaderMenu = createShaderMenu.CreateWidget<VisionUI::Widgets::Menu::MenuList>("Unlit template");
			auto& createLambertShaderMenu = createShaderMenu.CreateWidget<VisionUI::Widgets::Menu::MenuList>("Lambert template");

			auto& createEmptyMaterialMenu = createMaterialMenu.CreateWidget<VisionUI::Widgets::Menu::MenuList>("Empty");
			auto& createStandardMaterialMenu = createMaterialMenu.CreateWidget<VisionUI::Widgets::Menu::MenuList>("Standard");
			auto& createStandardPBRMaterialMenu = createMaterialMenu.CreateWidget<VisionUI::Widgets::Menu::MenuList>("Standard PBR");
			auto& createUnlitMaterialMenu = createMaterialMenu.CreateWidget<VisionUI::Widgets::Menu::MenuList>("Unlit");
			auto& createLambertMaterialMenu = createMaterialMenu.CreateWidget<VisionUI::Widgets::Menu::MenuList>("Lambert");

			auto& createFolder = createFolderMenu.CreateWidget<VisionUI::Widgets::InputFields::InputText>("");
			auto& createScene = createSceneMenu.CreateWidget<VisionUI::Widgets::InputFields::InputText>("");

			auto& createEmptyMaterial = createEmptyMaterialMenu.CreateWidget<VisionUI::Widgets::InputFields::InputText>("");
			auto& createStandardMaterial = createStandardMaterialMenu.CreateWidget<VisionUI::Widgets::InputFields::InputText>("");
			auto& createStandardPBRMaterial = createStandardPBRMaterialMenu.CreateWidget<VisionUI::Widgets::InputFields::InputText>("");
			auto& createUnlitMaterial = createUnlitMaterialMenu.CreateWidget<VisionUI::Widgets::InputFields::InputText>("");
			auto& createLambertMaterial = createLambertMaterialMenu.CreateWidget<VisionUI::Widgets::InputFields::InputText>("");

			auto& createStandardShader = createStandardShaderMenu.CreateWidget<VisionUI::Widgets::InputFields::InputText>("");
			auto& createStandardPBRShader = createStandardPBRShaderMenu.CreateWidget<VisionUI::Widgets::InputFields::InputText>("");
			auto& createUnlitShader = createUnlitShaderMenu.CreateWidget<VisionUI::Widgets::InputFields::InputText>("");
			auto& createLambertShader = createLambertShaderMenu.CreateWidget<VisionUI::Widgets::InputFields::InputText>("");

			createFolderMenu.ClickedEvent += [&createFolder] { createFolder.content = ""; };
			createSceneMenu.ClickedEvent += [&createScene] { createScene.content = ""; };
			createStandardShaderMenu.ClickedEvent += [&createStandardShader] { createStandardShader.content = ""; };
			createStandardPBRShaderMenu.ClickedEvent += [&createStandardPBRShader] { createStandardPBRShader.content = ""; };
			createUnlitShaderMenu.ClickedEvent += [&createUnlitShader] { createUnlitShader.content = ""; };
			createLambertShaderMenu.ClickedEvent += [&createLambertShader] { createLambertShader.content = ""; };
			createEmptyMaterialMenu.ClickedEvent += [&createEmptyMaterial] { createEmptyMaterial.content = ""; };
			createStandardMaterialMenu.ClickedEvent += [&createStandardMaterial] { createStandardMaterial.content = ""; };
			createStandardPBRMaterialMenu.ClickedEvent += [&createStandardPBRMaterial] { createStandardPBRMaterial.content = ""; };
			createUnlitMaterialMenu.ClickedEvent += [&createUnlitMaterial] { createUnlitMaterial.content = ""; };
			createLambertMaterialMenu.ClickedEvent += [&createLambertMaterial] { createLambertMaterial.content = ""; };

			createFolder.EnterPressedEvent += [this](std::string newFolderName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + (!fails ? newFolderName : newFolderName + " (" + std::to_string(fails) + ')');

					++fails;
				} while (std::filesystem::exists(finalPath));

				std::filesystem::create_directory(finalPath);

				ItemAddedEvent.Invoke(finalPath);
				Close();
			};

			createScene.EnterPressedEvent += [this](std::string newSceneName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + (!fails ? newSceneName : newSceneName + " (" + std::to_string(fails) + ')') + ".scene";

					++fails;
				} while (std::filesystem::exists(finalPath));

				std::ofstream outfile(finalPath);
				outfile << "<root><scene><actors><actor><name>Directional Light</name><tag></tag><active>true</active><id>1</id><parent>0</parent><components><component><type>class VisionCore::ECS::Components::CDirectionalLight</type><data><diffuse><x>1</x><y>1</y><z>1</z></diffuse><specular><x>1</x><y>1</y><z>1</z></specular><intensity>0.75</intensity></data></component><component><type>class VisionCore::ECS::Components::CTransform</type><data><position><x>0</x><y>10</y><z>0</z></position><rotation><x>0.81379771</x><y>-0.17101006</y><z>0.29619816</z><w>0.46984628</w></rotation><scale><x>1</x><y>1</y><z>1</z></scale></data></component></components><behaviours/></actor><actor><name>Ambient Light</name><tag></tag><active>true</active><id>2</id><parent>0</parent><components><component><type>class VisionCore::ECS::Components::CAmbientSphereLight</type><data><ambient><x>1</x><y>1</y><z>1</z></ambient><intensity>0.1</intensity><radius>10000</radius></data></component><component><type>class VisionCore::ECS::Components::CTransform</type><data><position><x>0</x><y>0</y><z>0</z></position><rotation><x>0</x><y>0</y><z>0</z><w>1</w></rotation><scale><x>1</x><y>1</y><z>1</z></scale></data></component></components><behaviours/></actor><actor><name>Main Camera</name><tag></tag><active>true</active><id>3</id><parent>0</parent><components><component><type>class VisionCore::ECS::Components::CCamera</type><data><fov>45</fov><near>0.1</near><far>1000</far><clear_color><x>0.1921569</x><y>0.3019608</y><z>0.47450981</z></clear_color></data></component><component><type>class VisionCore::ECS::Components::CTransform</type><data><position><x>0</x><y>3</y><z>8</z></position><rotation><x>-7.5904039e-09</x><y>0.98480773</y><z>-0.17364819</z><w>-4.3047311e-08</w></rotation><scale><x>1</x><y>1</y><z>1</z></scale></data></component></components><behaviours/></actor></actors></scene></root>" << std::endl; // Empty scene content

				ItemAddedEvent.Invoke(finalPath);
				Close();
			};

			createStandardShader.EnterPressedEvent += [this](std::string newShaderName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + '\\' + (!fails ? newShaderName : newShaderName + " (" + std::to_string(fails) + ')') + ".glsl";

					++fails;
				} while (std::filesystem::exists(finalPath));

				std::filesystem::copy_file(EDITOR_CONTEXT(engineAssetsPath) + "Shaders\\Standard.glsl", finalPath);
				ItemAddedEvent.Invoke(finalPath);
				Close();
			};

			createStandardPBRShader.EnterPressedEvent += [this](std::string newShaderName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + '\\' + (!fails ? newShaderName : newShaderName + " (" + std::to_string(fails) + ')') + ".glsl";

					++fails;
				} while (std::filesystem::exists(finalPath));

				std::filesystem::copy_file(EDITOR_CONTEXT(engineAssetsPath) + "Shaders\\StandardPBR.glsl", finalPath);
				ItemAddedEvent.Invoke(finalPath);
				Close();
			};

			createUnlitShader.EnterPressedEvent += [this](std::string newShaderName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + '\\' + (!fails ? newShaderName : newShaderName + " (" + std::to_string(fails) + ')') + ".glsl";

					++fails;
				} while (std::filesystem::exists(finalPath));

				std::filesystem::copy_file(EDITOR_CONTEXT(engineAssetsPath) + "Shaders\\Unlit.glsl", finalPath);
				ItemAddedEvent.Invoke(finalPath);
				Close();
			};

			createLambertShader.EnterPressedEvent += [this](std::string newShaderName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + '\\' + (!fails ? newShaderName : newShaderName + " (" + std::to_string(fails) + ')') + ".glsl";

					++fails;
				} while (std::filesystem::exists(finalPath));

				std::filesystem::copy_file(EDITOR_CONTEXT(engineAssetsPath) + "Shaders\\Lambert.glsl", finalPath);
				ItemAddedEvent.Invoke(finalPath);
				Close();
			};
			
			createEmptyMaterial.EnterPressedEvent += [this](std::string materialName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".mat";

					++fails;
				} while (std::filesystem::exists(finalPath));

				{
					std::ofstream outfile(finalPath);
					outfile << "<root><shader>?</shader></root>" << std::endl; // Empty material content
				}

				ItemAddedEvent.Invoke(finalPath);

				if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
				{
					auto& materialEditor = EDITOR_PANEL(VisionEditor::Panels::MaterialEditor, "Material Editor");
					materialEditor.SetTarget(*instance);
					materialEditor.Open();
					materialEditor.Focus();
					materialEditor.Preview();
				}
				Close();
			};

			createStandardMaterial.EnterPressedEvent += [this](std::string materialName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".mat";

					++fails;
				} while (std::filesystem::exists(finalPath));

				{
					std::ofstream outfile(finalPath);
					outfile << "<root><shader>:Shaders\\Standard.glsl</shader></root>" << std::endl; // Empty standard material content
				}

				ItemAddedEvent.Invoke(finalPath);

				if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
				{
					auto& materialEditor = EDITOR_PANEL(VisionEditor::Panels::MaterialEditor, "Material Editor");
					materialEditor.SetTarget(*instance);
					materialEditor.Open();
					materialEditor.Focus();
					materialEditor.Preview();
				}
				Close();
			};

			createStandardPBRMaterial.EnterPressedEvent += [this](std::string materialName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".mat";

					++fails;
				} while (std::filesystem::exists(finalPath));

				{
					std::ofstream outfile(finalPath);
					outfile << "<root><shader>:Shaders\\StandardPBR.glsl</shader></root>" << std::endl; // Empty standard material content
				}

				ItemAddedEvent.Invoke(finalPath);

				if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
				{
					auto& materialEditor = EDITOR_PANEL(VisionEditor::Panels::MaterialEditor, "Material Editor");
					materialEditor.SetTarget(*instance);
					materialEditor.Open();
					materialEditor.Focus();
					materialEditor.Preview();
				}
				Close();
			};

			createUnlitMaterial.EnterPressedEvent += [this](std::string materialName)
			{
				std::string newSceneName = "Material";
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".mat";

					++fails;
				} while (std::filesystem::exists(finalPath));

				{
					std::ofstream outfile(finalPath);
					outfile << "<root><shader>:Shaders\\Unlit.glsl</shader></root>" << std::endl; // Empty unlit material content
				}

				ItemAddedEvent.Invoke(finalPath);

				if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
				{
					auto& materialEditor = EDITOR_PANEL(VisionEditor::Panels::MaterialEditor, "Material Editor");
					materialEditor.SetTarget(*instance);
					materialEditor.Open();
					materialEditor.Focus();
					materialEditor.Preview();
				}
				Close();
			};

			createLambertMaterial.EnterPressedEvent += [this](std::string materialName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".mat";

					++fails;
				} while (std::filesystem::exists(finalPath));

				{
					std::ofstream outfile(finalPath);
					outfile << "<root><shader>:Shaders\\Lambert.glsl</shader></root>" << std::endl; // Empty unlit material content
				}

				ItemAddedEvent.Invoke(finalPath);

				if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
				{
					auto& materialEditor = EDITOR_PANEL(VisionEditor::Panels::MaterialEditor, "Material Editor");
					materialEditor.SetTarget(*instance);
					materialEditor.Open();
					materialEditor.Focus();
					materialEditor.Preview();
				}
				Close();
			};

			BrowserItemContextualMenu::CreateList();
		}
	}

	virtual void DeleteItem() override
	{
		using namespace VisionWindowing::Dialogs;
		MessageBox message("Delete folder", "Deleting a folder (and all its content) is irreversible, are you sure that you want to delete \"" + filePath + "\"?", MessageBox::EMessageType::WARNING, MessageBox::EButtonLayout::YES_NO);

		if (message.GetUserAction() == MessageBox::EUserAction::YES)
		{
			if (std::filesystem::exists(filePath) == true)
			{
				EDITOR_EXEC(PropagateFolderDestruction(filePath));
				std::filesystem::remove_all(filePath);
				DestroyedEvent.Invoke(filePath);
			}
		}
	}

public:
	VisionTools::Eventing::Event<std::string> ItemAddedEvent;
};

class ScriptFolderContextualMenu : public FolderContextualMenu
{
public:
	ScriptFolderContextualMenu(const std::string& p_filePath, bool p_protected = false) : FolderContextualMenu(p_filePath, p_protected) {}

	void CreateScript(const std::string& p_name, const std::string& p_path)
	{
		std::string fileContent = "local " + p_name + " =\n{\n}\n\nfunction " + p_name + ":OnStart()\nend\n\nfunction " + p_name + ":OnUpdate(deltaTime)\nend\n\nreturn " + p_name;
		
		std::ofstream outfile(p_path);
		outfile << fileContent << std::endl; // Empty scene content

		ItemAddedEvent.Invoke(p_path);
		Close();
	}

	virtual void CreateList() override
	{
		FolderContextualMenu::CreateList();

		auto& newScriptMenu = CreateWidget<VisionUI::Widgets::Menu::MenuList>("New script...");
		auto& nameEditor = newScriptMenu.CreateWidget<VisionUI::Widgets::InputFields::InputText>("");

		newScriptMenu.ClickedEvent += [this, &nameEditor]
		{
			nameEditor.content = VisionTools::Utils::PathParser::GetElementName("");
		};

		nameEditor.EnterPressedEvent += [this](std::string p_newName)
		{
			/* Clean the name (Remove special chars) */
			p_newName.erase(std::remove_if(p_newName.begin(), p_newName.end(), [](auto& c)
			{
				return std::find(FILENAMES_CHARS.begin(), FILENAMES_CHARS.end(), c) == FILENAMES_CHARS.end();
			}), p_newName.end());

			std::string newPath = filePath + p_newName + ".lua";

			if (!std::filesystem::exists(newPath))
			{
				CreateScript(p_newName, newPath);
			}
		};
	}
};

class FileContextualMenu : public BrowserItemContextualMenu
{
public:
	FileContextualMenu(const std::string& p_filePath, bool p_protected = false) : BrowserItemContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& editAction = CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Open");

		editAction.ClickedEvent += [this]
		{
			VisionTools::Utils::SystemCalls::OpenFile(filePath);
		};

		if (!m_protected)
		{
			auto& duplicateAction = CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Duplicate");

			duplicateAction.ClickedEvent += [this]
			{
				std::string filePathWithoutExtension = filePath;

				if (size_t pos = filePathWithoutExtension.rfind('.'); pos != std::string::npos)
					filePathWithoutExtension = filePathWithoutExtension.substr(0, pos);

				std::string extension = "." + VisionTools::Utils::PathParser::GetExtension(filePath);

                auto filenameAvailable = [&extension](const std::string& target)
                {
                    return !std::filesystem::exists(target + extension);
                };

                const auto newNameWithoutExtension = VisionTools::Utils::String::GenerateUnique(filePathWithoutExtension, filenameAvailable);

				std::string finalPath = newNameWithoutExtension + extension;
				std::filesystem::copy(filePath, finalPath);

				DuplicateEvent.Invoke(finalPath);
			};
		}

		BrowserItemContextualMenu::CreateList();


        auto& editMetadata = CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Properties");

        editMetadata.ClickedEvent += [this]
        {
            auto& panel = EDITOR_PANEL(VisionEditor::Panels::AssetProperties, "Asset Properties");
            std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
            panel.SetTarget(resourcePath);
            panel.Open();
            panel.Focus();
        };
	}

	virtual void DeleteItem() override
	{
		using namespace VisionWindowing::Dialogs;
		MessageBox message("Delete file", "Deleting a file is irreversible, are you sure that you want to delete \"" + filePath + "\"?", MessageBox::EMessageType::WARNING, MessageBox::EButtonLayout::YES_NO);

		if (message.GetUserAction() == MessageBox::EUserAction::YES)
		{
			RemoveAsset(filePath);
			DestroyedEvent.Invoke(filePath);
			EDITOR_EXEC(PropagateFileRename(filePath, "?"));
		}
	}

public:
	VisionTools::Eventing::Event<std::string> DuplicateEvent;
};

template<typename Resource, typename ResourceLoader>
class PreviewableContextualMenu : public FileContextualMenu
{
public:
	PreviewableContextualMenu(const std::string& p_filePath, bool p_protected = false) : FileContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& previewAction = CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Preview");

		previewAction.ClickedEvent += [this]
		{
			Resource* resource = VisionCore::Global::ServiceLocator::Get<ResourceLoader>()[EDITOR_EXEC(GetResourcePath(filePath, m_protected))];
			auto& assetView = EDITOR_PANEL(VisionEditor::Panels::AssetView, "Asset View");
			assetView.SetResource(resource);
			assetView.Open();
			assetView.Focus();
		};

		FileContextualMenu::CreateList();
	}
};

class ShaderContextualMenu : public FileContextualMenu
{
public:
	ShaderContextualMenu(const std::string& p_filePath, bool p_protected = false) : FileContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		FileContextualMenu::CreateList();

		auto& compileAction = CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Compile");

		compileAction.ClickedEvent += [this]
		{
			auto& shaderManager = SERVICE(VisionCore::ResourceManagement::ShaderManager);
			std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
			if (shaderManager.IsResourceRegistered(resourcePath))
			{
				/* Trying to recompile */
				VisionRendering::Resources::Loaders::ShaderLoader::Recompile(*shaderManager[resourcePath], filePath);
			}
			else
			{
				/* Trying to compile */
				VisionRendering::Resources::Shader* shader = SERVICE(VisionCore::ResourceManagement::ShaderManager)[resourcePath];
				if (shader)
					LOG_INFO("[COMPILE] \"" + filePath + "\": Success!");
			}
			
		};
	}
};

class ModelContextualMenu : public PreviewableContextualMenu<VisionRendering::Resources::Model, VisionCore::ResourceManagement::ModelManager>
{
public:
	ModelContextualMenu(const std::string& p_filePath, bool p_protected = false) : PreviewableContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& reloadAction = CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Reload");

		reloadAction.ClickedEvent += [this]
		{
			auto& modelManager = SERVICE(VisionCore::ResourceManagement::ModelManager);
			std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
			if (modelManager.IsResourceRegistered(resourcePath))
			{
				modelManager.AResourceManager::ReloadResource(resourcePath);
			}
		};

		if (!m_protected)
		{
			auto& generateMaterialsMenu = CreateWidget<VisionUI::Widgets::Menu::MenuList>("Generate materials...");

			generateMaterialsMenu.CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Standard").ClickedEvent += [this]
			{
				auto& modelManager = SERVICE(VisionCore::ResourceManagement::ModelManager);
				std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
				if (auto model = modelManager.GetResource(resourcePath))
				{
					for (const std::string& materialName : model->GetMaterialNames())
					{
						size_t fails = 0;
						std::string finalPath;

						do
						{
							finalPath = VisionTools::Utils::PathParser::GetContainingFolder(filePath) + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".mat";

							++fails;
						} while (std::filesystem::exists(finalPath));

						{
							std::ofstream outfile(finalPath);
							outfile << "<root><shader>:Shaders\\Standard.glsl</shader></root>" << std::endl; // Empty standard material content
						}

						DuplicateEvent.Invoke(finalPath);
					}
				}
			};

			generateMaterialsMenu.CreateWidget<VisionUI::Widgets::Menu::MenuItem>("StandardPBR").ClickedEvent += [this]
			{
				auto& modelManager = SERVICE(VisionCore::ResourceManagement::ModelManager);
				std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
				if (auto model = modelManager.GetResource(resourcePath))
				{
					for (const std::string& materialName : model->GetMaterialNames())
					{
						size_t fails = 0;
						std::string finalPath;

						do
						{
							finalPath = VisionTools::Utils::PathParser::GetContainingFolder(filePath) + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".mat";

							++fails;
						} while (std::filesystem::exists(finalPath));

						{
							std::ofstream outfile(finalPath);
							outfile << "<root><shader>:Shaders\\StandardPBR.glsl</shader></root>" << std::endl; // Empty standard material content
						}

						DuplicateEvent.Invoke(finalPath);
					}
				}
			};

			generateMaterialsMenu.CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Unlit").ClickedEvent += [this]
			{
				auto& modelManager = SERVICE(VisionCore::ResourceManagement::ModelManager);
				std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
				if (auto model = modelManager.GetResource(resourcePath))
				{
					for (const std::string& materialName : model->GetMaterialNames())
					{
						size_t fails = 0;
						std::string finalPath;

						do
						{
							finalPath = VisionTools::Utils::PathParser::GetContainingFolder(filePath) + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".mat";

							++fails;
						} while (std::filesystem::exists(finalPath));

						{
							std::ofstream outfile(finalPath);
							outfile << "<root><shader>:Shaders\\Unlit.glsl</shader></root>" << std::endl; // Empty standard material content
						}

						DuplicateEvent.Invoke(finalPath);
					}
				}
			};

			generateMaterialsMenu.CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Lambert").ClickedEvent += [this]
			{
				auto& modelManager = SERVICE(VisionCore::ResourceManagement::ModelManager);
				std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
				if (auto model = modelManager.GetResource(resourcePath))
				{
					for (const std::string& materialName : model->GetMaterialNames())
					{
						size_t fails = 0;
						std::string finalPath;

						do
						{
							finalPath = VisionTools::Utils::PathParser::GetContainingFolder(filePath) + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".mat";

							++fails;
						} while (std::filesystem::exists(finalPath));

						{
							std::ofstream outfile(finalPath);
							outfile << "<root><shader>:Shaders\\Lambert.glsl</shader></root>" << std::endl; // Empty standard material content
						}

						DuplicateEvent.Invoke(finalPath);
					}
				}
			};
		}

		PreviewableContextualMenu::CreateList();
	}
};

class TextureContextualMenu : public PreviewableContextualMenu<VisionRendering::Resources::Texture, VisionCore::ResourceManagement::TextureManager>
{
public:
	TextureContextualMenu(const std::string& p_filePath, bool p_protected = false) : PreviewableContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& reloadAction = CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Reload");

		reloadAction.ClickedEvent += [this]
		{
			auto& textureManager = SERVICE(VisionCore::ResourceManagement::TextureManager);
			std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
			if (textureManager.IsResourceRegistered(resourcePath))
			{
				/* Trying to recompile */
				textureManager.AResourceManager::ReloadResource(resourcePath);
				EDITOR_PANEL(VisionEditor::Panels::MaterialEditor, "Material Editor").Refresh();
			}
		};

		PreviewableContextualMenu::CreateList();
	}
};

class SceneContextualMenu : public FileContextualMenu
{
public:
	SceneContextualMenu(const std::string& p_filePath, bool p_protected = false) : FileContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& editAction = CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Edit");

		editAction.ClickedEvent += [this]
		{
			EDITOR_EXEC(LoadSceneFromDisk(EDITOR_EXEC(GetResourcePath(filePath))));
		};

		FileContextualMenu::CreateList();
	}
};

class MaterialContextualMenu : public PreviewableContextualMenu<VisionCore::Resources::Material, VisionCore::ResourceManagement::MaterialManager>
{
public:
	MaterialContextualMenu(const std::string& p_filePath, bool p_protected = false) : PreviewableContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& editAction = CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Edit");

		editAction.ClickedEvent += [this]
		{
			VisionCore::Resources::Material* material = SERVICE(VisionCore::ResourceManagement::MaterialManager)[EDITOR_EXEC(GetResourcePath(filePath, m_protected))];
			if (material)
			{
				auto& materialEditor = EDITOR_PANEL(VisionEditor::Panels::MaterialEditor, "Material Editor");
				materialEditor.SetTarget(*material);
				materialEditor.Open();
				materialEditor.Focus();
				
				VisionCore::Resources::Material* resource = VisionCore::Global::ServiceLocator::Get<VisionCore::ResourceManagement::MaterialManager>()[EDITOR_EXEC(GetResourcePath(filePath, m_protected))];
				auto& assetView = EDITOR_PANEL(VisionEditor::Panels::AssetView, "Asset View");
				assetView.SetResource(resource);
				assetView.Open();
				assetView.Focus();
			}
		};

		auto& reload = CreateWidget<VisionUI::Widgets::Menu::MenuItem>("Reload");
		reload.ClickedEvent += [this]
		{
			auto materialManager = SERVICE(VisionCore::ResourceManagement::MaterialManager);
			auto resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
			VisionCore::Resources::Material* material = materialManager[resourcePath];
			if (material)
			{
				materialManager.AResourceManager::ReloadResource(resourcePath);
				EDITOR_PANEL(VisionEditor::Panels::MaterialEditor, "Material Editor").Refresh();
			}
		};

		PreviewableContextualMenu::CreateList();
	}
};

void OnFileChanged(std::string file)
{
	switch (VisionTools::Utils::PathParser::GetFileType(file))
	{
	case VisionTools::Utils::PathParser::EFileType::SHADER:
		auto& shaderManager = SERVICE(VisionCore::ResourceManagement::ShaderManager);
		std::string resourcePath = EDITOR_EXEC(GetResourcePath(file));
		if (shaderManager.IsResourceRegistered(resourcePath))
		{
			LOG_INFO("Shader changed (" + file + "), automatic shader recompilation called");
			VisionRendering::Resources::Loaders::ShaderLoader::Recompile(*shaderManager[resourcePath], file);
		}
		break;
	}
}

VisionEditor::Panels::AssetBrowser::AssetBrowser
(
	const std::string& p_title,
	bool p_opened,
	const VisionUI::Settings::PanelWindowSettings& p_windowSettings,
	const std::string& p_engineAssetFolder,
	const std::string& p_projectAssetFolder,
	const std::string& p_projectScriptFolder
) :
	PanelWindow(p_title, p_opened, p_windowSettings),
	m_engineAssetFolder(p_engineAssetFolder),
	m_projectAssetFolder(p_projectAssetFolder),
	m_projectScriptFolder(p_projectScriptFolder),
	m_projectAssetWatcher(m_projectAssetFolder, std::chrono::milliseconds(2000))
{
	if (!std::filesystem::exists(m_projectAssetFolder))
	{
		std::filesystem::create_directories(m_projectAssetFolder);

		VisionWindowing::Dialogs::MessageBox message
		(
			"Assets folder not found",
			"The \"Assets/\" folders hasn't been found in your project directory.\nIt has been automatically generated",
			VisionWindowing::Dialogs::MessageBox::EMessageType::WARNING,
			VisionWindowing::Dialogs::MessageBox::EButtonLayout::OK
		);
	}

	if (!std::filesystem::exists(m_projectScriptFolder))
	{
		std::filesystem::create_directories(m_projectScriptFolder);

		VisionWindowing::Dialogs::MessageBox message
		(
			"Scripts folder not found",
			"The \"Scripts/\" folders hasn't been found in your project directory.\nIt has been automatically generated",
			VisionWindowing::Dialogs::MessageBox::EMessageType::WARNING,
			VisionWindowing::Dialogs::MessageBox::EButtonLayout::OK
		);
	}

	auto& refreshButton = CreateWidget<Buttons::Button>("Rescan assets");
	refreshButton.ClickedEvent += std::bind(&AssetBrowser::Refresh, this);
	refreshButton.lineBreak = false;
	refreshButton.idleBackgroundColor = { 0.f, 0.5f, 0.0f };

	auto& importButton = CreateWidget<Buttons::Button>("Import asset");
	importButton.ClickedEvent += EDITOR_BIND(ImportAsset, m_projectAssetFolder);
	importButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	m_assetList = &CreateWidget<Layout::Group>();

	Fill();

	m_projectAssetWatcher.FileChangedEvent += [](const std::string& file)
	{
		EDITOR_EXEC(DelayAction(std::bind(&OnFileChanged, file), 0));
	};
	m_projectAssetWatcher.Start();
}

void VisionEditor::Panels::AssetBrowser::Fill()
{
	m_assetList->CreateWidget<VisionUI::Widgets::Visual::Separator>();
	ConsiderItem(nullptr, std::filesystem::directory_entry(m_engineAssetFolder), true);
	m_assetList->CreateWidget<VisionUI::Widgets::Visual::Separator>();
	ConsiderItem(nullptr, std::filesystem::directory_entry(m_projectAssetFolder), false);
	m_assetList->CreateWidget<VisionUI::Widgets::Visual::Separator>();
	ConsiderItem(nullptr, std::filesystem::directory_entry(m_projectScriptFolder), false, false, true);
}

void VisionEditor::Panels::AssetBrowser::Clear()
{
	m_assetList->RemoveAllWidgets();
}

void VisionEditor::Panels::AssetBrowser::Refresh()
{
	Clear();
	Fill();
}

void VisionEditor::Panels::AssetBrowser::ParseFolder(Layout::TreeNode& p_root, const std::filesystem::directory_entry& p_directory, bool p_isEngineItem, bool p_scriptFolder)
{
	/* Iterates another time to display list files */
	for (auto& item : std::filesystem::directory_iterator(p_directory))
		if (item.is_directory())
			ConsiderItem(&p_root, item, p_isEngineItem, false, p_scriptFolder);

	/* Iterates another time to display list files */
	for (auto& item : std::filesystem::directory_iterator(p_directory))
		if (!item.is_directory())
			ConsiderItem(&p_root, item, p_isEngineItem, false, p_scriptFolder);
}

void VisionEditor::Panels::AssetBrowser::ConsiderItem(VisionUI::Widgets::Layout::TreeNode* p_root, const std::filesystem::directory_entry& p_entry, bool p_isEngineItem, bool p_autoOpen, bool p_scriptFolder)
{
	bool isDirectory = p_entry.is_directory();
	std::string itemname = VisionTools::Utils::PathParser::GetElementName(p_entry.path().string());
	std::string path = p_entry.path().string();
	if (isDirectory && path.back() != '\\') // Add '\\' if is directory and backslash is missing
		path += '\\';
	std::string resourceFormatPath = EDITOR_EXEC(GetResourcePath(path, p_isEngineItem));
	bool protectedItem = !p_root || p_isEngineItem;

	VisionTools::Utils::PathParser::EFileType fileType = VisionTools::Utils::PathParser::GetFileType(itemname);

	// Unknown file, so we skip it
	if (fileType == VisionTools::Utils::PathParser::EFileType::UNKNOWN && !isDirectory)
	{
		return;
	}

	/* If there is a given treenode (p_root) we attach the new widget to it */
	auto& itemGroup = p_root ? p_root->CreateWidget<Layout::Group>() : m_assetList->CreateWidget<Layout::Group>();

	/* Find the icon to apply to the item */
	uint32_t iconTextureID = isDirectory ? EDITOR_CONTEXT(editorResources)->GetTexture("Icon_Folder")->id : EDITOR_CONTEXT(editorResources)->GetFileIcon(itemname)->id;

	itemGroup.CreateWidget<Visual::Image>(iconTextureID, VisionMaths::FVector2{ 16, 16 }).lineBreak = false;

	/* If the entry is a directory, the content must be a tree node, otherwise (= is a file), a text will suffice */
	if (isDirectory)
	{
		auto& treeNode = itemGroup.CreateWidget<Layout::TreeNode>(itemname);

		if (p_autoOpen)
			treeNode.Open();

		auto& ddSource = treeNode.AddPlugin<VisionUI::Plugins::DDSource<std::pair<std::string, Layout::Group*>>>("Folder", resourceFormatPath, std::make_pair(resourceFormatPath, &itemGroup));
		
		if (!p_root || p_scriptFolder)
			treeNode.RemoveAllPlugins();

		auto& contextMenu = !p_scriptFolder ? treeNode.AddPlugin<FolderContextualMenu>(path, protectedItem && resourceFormatPath != "") : treeNode.AddPlugin<ScriptFolderContextualMenu>(path, protectedItem && resourceFormatPath != "");
		contextMenu.userData = static_cast<void*>(&treeNode);

		contextMenu.ItemAddedEvent += [this, &treeNode, path, p_isEngineItem, p_scriptFolder] (std::string p_string)
		{
			treeNode.Open();
			treeNode.RemoveAllWidgets();
			ParseFolder(treeNode, std::filesystem::directory_entry(VisionTools::Utils::PathParser::GetContainingFolder(p_string)), p_isEngineItem, p_scriptFolder);
		};

		if (!p_scriptFolder)
		{
			if (!p_isEngineItem) /* Prevent engine item from being DDTarget (Can't Drag and drop to engine folder) */
			{
				treeNode.AddPlugin<VisionUI::Plugins::DDTarget<std::pair<std::string, Layout::Group*>>>("Folder").DataReceivedEvent += [this, &treeNode, path, p_isEngineItem](std::pair<std::string, Layout::Group*> p_data)
				{
					if (!p_data.first.empty())
					{
						std::string folderReceivedPath = EDITOR_EXEC(GetRealPath(p_data.first));

						std::string folderName = VisionTools::Utils::PathParser::GetElementName(folderReceivedPath) + '\\';
						std::string prevPath = folderReceivedPath;
						std::string correctPath = m_pathUpdate.find(&treeNode) != m_pathUpdate.end() ? m_pathUpdate.at(&treeNode) : path;
						std::string newPath = correctPath + folderName;

						if (!(newPath.find(prevPath) != std::string::npos) || prevPath == newPath)
						{
							if (!std::filesystem::exists(newPath))
							{
								bool isEngineFolder = p_data.first.at(0) == ':';

								if (isEngineFolder) /* Copy dd folder from Engine resources */
									std::filesystem::copy(prevPath, newPath, std::filesystem::copy_options::recursive);
								else
								{
									RenameAsset(prevPath, newPath);
									EDITOR_EXEC(PropagateFolderRename(prevPath, newPath));
								}

								treeNode.Open();
								treeNode.RemoveAllWidgets();
								ParseFolder(treeNode, std::filesystem::directory_entry(correctPath), p_isEngineItem);

								if (!isEngineFolder)
									p_data.second->Destroy();
							}
							else if (prevPath == newPath)
							{
								// Ignore
							}
							else
							{
								using namespace VisionWindowing::Dialogs;

								MessageBox errorMessage("Folder already exists", "You can't move this folder to this location because the name is already taken", MessageBox::EMessageType::ERROR, MessageBox::EButtonLayout::OK);
							}
						}
						else
						{
							using namespace VisionWindowing::Dialogs;

							MessageBox errorMessage("Wow!", "Crazy boy!", MessageBox::EMessageType::ERROR, MessageBox::EButtonLayout::OK);
						}
					}
				};

				treeNode.AddPlugin<VisionUI::Plugins::DDTarget<std::pair<std::string, Layout::Group*>>>("File").DataReceivedEvent += [this, &treeNode, path, p_isEngineItem](std::pair<std::string, Layout::Group*> p_data)
				{
					if (!p_data.first.empty())
					{
						std::string fileReceivedPath = EDITOR_EXEC(GetRealPath(p_data.first));

						std::string fileName = VisionTools::Utils::PathParser::GetElementName(fileReceivedPath);
						std::string prevPath = fileReceivedPath;
						std::string correctPath = m_pathUpdate.find(&treeNode) != m_pathUpdate.end() ? m_pathUpdate.at(&treeNode) : path;
						std::string newPath = correctPath + fileName;

						if (!std::filesystem::exists(newPath))
						{
							bool isEngineFile = p_data.first.at(0) == ':';

							if (isEngineFile) /* Copy dd file from Engine resources */
								std::filesystem::copy_file(prevPath, newPath);
							else
							{
								RenameAsset(prevPath, newPath);
								EDITOR_EXEC(PropagateFileRename(prevPath, newPath));
							}

							treeNode.Open();
							treeNode.RemoveAllWidgets();
							ParseFolder(treeNode, std::filesystem::directory_entry(correctPath), p_isEngineItem);

							if (!isEngineFile)
								p_data.second->Destroy();
						}
						else if (prevPath == newPath)
						{
							// Ignore
						}
						else
						{
							using namespace VisionWindowing::Dialogs;

							MessageBox errorMessage("File already exists", "You can't move this file to this location because the name is already taken", MessageBox::EMessageType::ERROR, MessageBox::EButtonLayout::OK);
						}
					}
				};
			}

			contextMenu.DestroyedEvent += [&itemGroup](std::string p_deletedPath) { itemGroup.Destroy(); };

			contextMenu.RenamedEvent += [this, &treeNode, path, &ddSource, p_isEngineItem](std::string p_prev, std::string p_newPath)
			{
				p_newPath += '\\';

				if (!std::filesystem::exists(p_newPath)) // Do not rename a folder if it already exists
				{
					RenameAsset(p_prev, p_newPath);
					EDITOR_EXEC(PropagateFolderRename(p_prev, p_newPath));
					std::string elementName = VisionTools::Utils::PathParser::GetElementName(p_newPath);
					std::string data = VisionTools::Utils::PathParser::GetContainingFolder(ddSource.data.first) + elementName + "\\";
					ddSource.data.first = data;
					ddSource.tooltip = data;
					treeNode.name = elementName;
					treeNode.Open();
					treeNode.RemoveAllWidgets();
					ParseFolder(treeNode, std::filesystem::directory_entry(p_newPath), p_isEngineItem);
					m_pathUpdate[&treeNode] = p_newPath;
				}
				else
				{
					using namespace VisionWindowing::Dialogs;

					MessageBox errorMessage("Folder already exists", "You can't rename this folder because the given name is already taken", MessageBox::EMessageType::ERROR, MessageBox::EButtonLayout::OK);
				}
			};

			contextMenu.ItemAddedEvent += [this, &treeNode, p_isEngineItem](std::string p_path)
			{
				treeNode.RemoveAllWidgets();
				ParseFolder(treeNode, std::filesystem::directory_entry(VisionTools::Utils::PathParser::GetContainingFolder(p_path)), p_isEngineItem);
			};

		}
		
		contextMenu.CreateList();

		treeNode.OpenedEvent += [this, &treeNode, path, p_isEngineItem, p_scriptFolder]
		{
			treeNode.RemoveAllWidgets();
			std::string updatedPath = VisionTools::Utils::PathParser::GetContainingFolder(path) + treeNode.name;
			ParseFolder(treeNode, std::filesystem::directory_entry(updatedPath), p_isEngineItem, p_scriptFolder);
		};

		treeNode.ClosedEvent += [this, &treeNode]
		{
			treeNode.RemoveAllWidgets();
		};
	}
	else
	{
		auto& clickableText = itemGroup.CreateWidget<Texts::TextClickable>(itemname);

		FileContextualMenu* contextMenu = nullptr;

		switch (fileType)
		{
		case VisionTools::Utils::PathParser::EFileType::MODEL:		contextMenu = &clickableText.AddPlugin<ModelContextualMenu>(path, protectedItem);		break;
		case VisionTools::Utils::PathParser::EFileType::TEXTURE:	contextMenu = &clickableText.AddPlugin<TextureContextualMenu>(path, protectedItem); 	break;
		case VisionTools::Utils::PathParser::EFileType::SHADER:		contextMenu = &clickableText.AddPlugin<ShaderContextualMenu>(path, protectedItem);		break;
		case VisionTools::Utils::PathParser::EFileType::MATERIAL:	contextMenu = &clickableText.AddPlugin<MaterialContextualMenu>(path, protectedItem);	break;
		case VisionTools::Utils::PathParser::EFileType::SCENE:		contextMenu = &clickableText.AddPlugin<SceneContextualMenu>(path, protectedItem);		break;
		default: contextMenu = &clickableText.AddPlugin<FileContextualMenu>(path, protectedItem); break;
		}

		contextMenu->CreateList();

		contextMenu->DestroyedEvent += [&itemGroup](std::string p_deletedPath)
		{
			itemGroup.Destroy();

			if (EDITOR_CONTEXT(sceneManager).GetCurrentSceneSourcePath() == p_deletedPath) // Modify current scene source path if the renamed file is the current scene
				EDITOR_CONTEXT(sceneManager).ForgetCurrentSceneSourcePath();
		};

		auto& ddSource = clickableText.AddPlugin<VisionUI::Plugins::DDSource<std::pair<std::string, Layout::Group*>>>
		(
			"File",
			resourceFormatPath,
			std::make_pair(resourceFormatPath, &itemGroup)
		);

		contextMenu->RenamedEvent += [&ddSource, &clickableText, p_scriptFolder](std::string p_prev, std::string p_newPath)
		{
			if (p_newPath != p_prev)
			{
				if (!std::filesystem::exists(p_newPath))
				{
					RenameAsset(p_prev, p_newPath);
					std::string elementName = VisionTools::Utils::PathParser::GetElementName(p_newPath);
					ddSource.data.first = VisionTools::Utils::PathParser::GetContainingFolder(ddSource.data.first) + elementName;

					if (!p_scriptFolder)
					{
						EDITOR_EXEC(PropagateFileRename(p_prev, p_newPath));
						if (EDITOR_CONTEXT(sceneManager).GetCurrentSceneSourcePath() == p_prev) // Modify current scene source path if the renamed file is the current scene
							EDITOR_CONTEXT(sceneManager).StoreCurrentSceneSourcePath(p_newPath);
					}
					else
					{
						EDITOR_EXEC(PropagateScriptRename(p_prev, p_newPath));
					}

					clickableText.content = elementName;
				}
				else
				{
					using namespace VisionWindowing::Dialogs;

					MessageBox errorMessage("File already exists", "You can't rename this file because the given name is already taken", MessageBox::EMessageType::ERROR, MessageBox::EButtonLayout::OK);
				}
			}
		};

		contextMenu->DuplicateEvent += [this, &clickableText, p_root, path, p_isEngineItem] (std::string newItem)
		{
			EDITOR_EXEC(DelayAction(std::bind(&AssetBrowser::ConsiderItem, this, p_root, std::filesystem::directory_entry{ newItem }, p_isEngineItem, false, false), 0));
		};

		if (fileType == VisionTools::Utils::PathParser::EFileType::TEXTURE)
		{
			auto& texturePreview = clickableText.AddPlugin<TexturePreview>();
			texturePreview.SetPath(resourceFormatPath);
		}
	}
}
