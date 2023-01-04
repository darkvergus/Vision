#include <VisionUI/Widgets/InputFields/InputInt.h>
#include <VisionUI/Widgets/Layout/Group.h>
#include <VisionUI/Widgets/Buttons/Button.h>
#include <VisionUI/Widgets/Buttons/ButtonSmall.h>
#include <VisionUI/Widgets/Texts/TextColored.h>
#include <VisionUI/Plugins/DDTarget.h>

#include <VisionTools/Utils/PathParser.h>

#include "VisionCore/ECS/Actor.h"
#include "VisionCore/ECS/Components/CMaterialRenderer.h"
#include "VisionCore/ECS/Components/CModelRenderer.h"
#include "VisionCore/ResourceManagement/MaterialManager.h"
#include "VisionCore/Global/ServiceLocator.h"

VisionCore::ECS::Components::CMaterialRenderer::CMaterialRenderer(ECS::Actor & p_owner) : AComponent(p_owner)
{
	m_materials.fill(nullptr);

	for (uint8_t i = 0; i < MAX_MATERIAL_COUNT; ++i)
		m_materialFields[i].fill(nullptr);

	UpdateMaterialList();
}

std::string VisionCore::ECS::Components::CMaterialRenderer::GetName()
{
	return "Material Renderer";
}

void VisionCore::ECS::Components::CMaterialRenderer::FillWithMaterial(VisionCore::Resources::Material & p_material)
{
	for (uint8_t i = 0; i < m_materials.size(); ++i)
		m_materials[i] = &p_material;
}

void VisionCore::ECS::Components::CMaterialRenderer::SetMaterialAtIndex(uint8_t p_index, VisionCore::Resources::Material& p_material)
{
	m_materials[p_index] = &p_material;
}

VisionCore::Resources::Material* VisionCore::ECS::Components::CMaterialRenderer::GetMaterialAtIndex(uint8_t p_index)
{
	return m_materials.at(p_index);
}

void VisionCore::ECS::Components::CMaterialRenderer::RemoveMaterialAtIndex(uint8_t p_index)
{
	if (p_index < m_materials.size())
	{
		m_materials[p_index] = nullptr;;
	}
}

void VisionCore::ECS::Components::CMaterialRenderer::RemoveMaterialByInstance(VisionCore::Resources::Material& p_instance)
{
	for (uint8_t i = 0; i < m_materials.size(); ++i)
		if (m_materials[i] == &p_instance)
			m_materials[i] = nullptr;
}

void VisionCore::ECS::Components::CMaterialRenderer::RemoveAllMaterials()
{
	for (uint8_t i = 0; i < m_materials.size(); ++i)
		m_materials[i] = nullptr;
}

const VisionMaths::FMatrix4 & VisionCore::ECS::Components::CMaterialRenderer::GetUserMatrix() const
{
	return m_userMatrix;
}

const VisionCore::ECS::Components::CMaterialRenderer::MaterialList& VisionCore::ECS::Components::CMaterialRenderer::GetMaterials() const
{
	return m_materials;
}

void VisionCore::ECS::Components::CMaterialRenderer::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	tinyxml2::XMLNode* materialsNode = p_doc.NewElement("materials");
	p_node->InsertEndChild(materialsNode);

	auto modelRenderer = owner.GetComponent<CModelRenderer>();
	uint8_t elementsToSerialize = modelRenderer->GetModel() ? (uint8_t)std::min(modelRenderer->GetModel()->GetMaterialNames().size(), (size_t)MAX_MATERIAL_COUNT) : 0;

	for (uint8_t i = 0; i < elementsToSerialize; ++i)
	{
		VisionCore::Helpers::Serializer::SerializeMaterial(p_doc, materialsNode, "material", m_materials[i]);
	}
}

void VisionCore::ECS::Components::CMaterialRenderer::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	tinyxml2::XMLNode* materialsRoot = p_node->FirstChildElement("materials");
	if (materialsRoot)
	{
		tinyxml2::XMLElement* currentMaterial = materialsRoot->FirstChildElement("material");

		uint8_t materialIndex = 0;

		while (currentMaterial)
		{
			if (auto material = Global::ServiceLocator::Get<ResourceManagement::MaterialManager>()[currentMaterial->GetText()])
				m_materials[materialIndex] = material;

			currentMaterial = currentMaterial->NextSiblingElement("material");
			++materialIndex;
		}
	}

	UpdateMaterialList();
}

std::array<VisionUI::Widgets::AWidget*, 3> CustomMaterialDrawer(VisionUI::Internal::WidgetContainer& p_root, const std::string& p_name, VisionCore::Resources::Material*& p_data)
{
	using namespace VisionCore::Helpers;

	std::array<VisionUI::Widgets::AWidget*, 3> widgets;

	widgets[0] = &p_root.CreateWidget<VisionUI::Widgets::Texts::TextColored>(p_name, GUIDrawer::TitleColor);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto & rightSide = p_root.CreateWidget<VisionUI::Widgets::Layout::Group>();

	auto& widget = rightSide.CreateWidget<VisionUI::Widgets::Texts::Text>(displayedText);

	widgets[1] = &widget;

	widget.AddPlugin<VisionUI::Plugins::DDTarget<std::pair<std::string, VisionUI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [&widget, &p_data](auto p_receivedData)
	{
		if (VisionTools::Utils::PathParser::GetFileType(p_receivedData.first) == VisionTools::Utils::PathParser::EFileType::MATERIAL)
		{
			if (auto resource = SERVICE(VisionCore::ResourceManagement::MaterialManager).GetResource(p_receivedData.first); resource)
			{
				p_data = resource;
				widget.content = p_receivedData.first;
			}
		}
	};

	widget.lineBreak = false;

	auto & resetButton = rightSide.CreateWidget<VisionUI::Widgets::Buttons::ButtonSmall>("Clear");
	resetButton.idleBackgroundColor = GUIDrawer::ClearButtonColor;
	resetButton.ClickedEvent += [&widget, &p_data]
	{
		p_data = nullptr;
		widget.content = "Empty";
	};

	widgets[2] = &resetButton;

	return widgets;
}

void VisionCore::ECS::Components::CMaterialRenderer::OnInspector(VisionUI::Internal::WidgetContainer & p_root)
{
	using namespace VisionCore::Helpers;

	for (uint8_t i = 0; i < m_materials.size(); ++i)
		m_materialFields[i] = CustomMaterialDrawer(p_root, "Material", m_materials[i]);

	UpdateMaterialList();
}

void VisionCore::ECS::Components::CMaterialRenderer::UpdateMaterialList()
{
	if (auto modelRenderer = owner.GetComponent<CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
	{
		uint8_t materialIndex = 0;

		for (const std::string& materialName : modelRenderer->GetModel()->GetMaterialNames())
		{
			m_materialNames[materialIndex++] = materialName;
		}

		for (uint8_t i = materialIndex; i < MAX_MATERIAL_COUNT; ++i)
			m_materialNames[i] = "";
	}

	for (uint8_t i = 0; i < m_materialFields.size(); ++i)
	{
		if (m_materialFields[i][0])
		{
			bool enabled = !m_materialNames[i].empty();
			m_materialFields[i][0]->enabled = enabled;
			m_materialFields[i][1]->enabled = enabled;
			m_materialFields[i][2]->enabled = enabled;
			reinterpret_cast<VisionUI::Widgets::Texts::Text*>(m_materialFields[i][0])->content = m_materialNames[i];
		}
	}
}

void VisionCore::ECS::Components::CMaterialRenderer::SetUserMatrixElement(uint32_t p_row, uint32_t p_column, float p_value)
{
	if (p_row < 4 && p_column < 4)
		m_userMatrix.data[4 * p_row + p_column] = p_value;
}

float VisionCore::ECS::Components::CMaterialRenderer::GetUserMatrixElement(uint32_t p_row, uint32_t p_column) const
{
	if (p_row < 4 && p_column < 4)
		return m_userMatrix.data[4 * p_row + p_column];
	else
		return 0.0f;
}
