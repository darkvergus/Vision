#include <VisionUI/Widgets/Texts/Text.h>
#include <VisionUI/Widgets/Texts/TextColored.h>
#include <VisionUI/Plugins/DDTarget.h>
#include <VisionUI/Widgets/Visual/Image.h>
#include <VisionUI/Widgets/Selection/ComboBox.h>
#include <VisionUI/Widgets/Selection/CheckBox.h>
#include <VisionUI/Widgets/Drags/DragFloat.h>

#include "VisionCore/Global/ServiceLocator.h"
#include "VisionCore/ResourceManagement/TextureManager.h"
#include "VisionCore/ResourceManagement/ModelManager.h"
#include "VisionCore/ResourceManagement/ShaderManager.h"
#include "VisionCore/ECS/Components/CModelRenderer.h"
#include "VisionCore/ECS/Components/CMaterialRenderer.h"
#include "VisionCore/ECS/Actor.h"

VisionCore::ECS::Components::CModelRenderer::CModelRenderer(ECS::Actor& p_owner) : AComponent(p_owner)
{
	m_modelChangedEvent += [this]
	{
		if (auto materialRenderer = owner.GetComponent<CMaterialRenderer>())
			materialRenderer->UpdateMaterialList();
	};
}

std::string VisionCore::ECS::Components::CModelRenderer::GetName()
{
	return "Model Renderer";
}

void VisionCore::ECS::Components::CModelRenderer::SetModel(VisionRendering::Resources::Model* p_model)
{
	m_model = p_model;
	m_modelChangedEvent.Invoke();
}

VisionRendering::Resources::Model * VisionCore::ECS::Components::CModelRenderer::GetModel() const
{
	return m_model;
}

void VisionCore::ECS::Components::CModelRenderer::SetFrustumBehaviour(EFrustumBehaviour p_boundingMode)
{
	m_frustumBehaviour = p_boundingMode;
}

VisionCore::ECS::Components::CModelRenderer::EFrustumBehaviour VisionCore::ECS::Components::CModelRenderer::GetFrustumBehaviour() const
{
	return m_frustumBehaviour;
}

const VisionRendering::Geometry::BoundingSphere& VisionCore::ECS::Components::CModelRenderer::GetCustomBoundingSphere() const
{
	return m_customBoundingSphere;
}

void VisionCore::ECS::Components::CModelRenderer::SetCustomBoundingSphere(const VisionRendering::Geometry::BoundingSphere& p_boundingSphere)
{
	m_customBoundingSphere = p_boundingSphere;
}

void VisionCore::ECS::Components::CModelRenderer::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	VisionCore::Helpers::Serializer::SerializeModel(p_doc, p_node, "model", m_model);
	VisionCore::Helpers::Serializer::SerializeInt(p_doc, p_node, "frustum_behaviour", reinterpret_cast<int&>(m_frustumBehaviour));
	VisionCore::Helpers::Serializer::SerializeVec3(p_doc, p_node, "custom_bounding_sphere_position", m_customBoundingSphere.position);
	VisionCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "custom_bounding_sphere_radius", m_customBoundingSphere.radius);
}

void VisionCore::ECS::Components::CModelRenderer::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode* p_node)
{
	VisionCore::Helpers::Serializer::DeserializeModel(p_doc, p_node, "model", m_model);
	VisionCore::Helpers::Serializer::DeserializeInt(p_doc, p_node, "frustum_behaviour", reinterpret_cast<int&>(m_frustumBehaviour));
	VisionCore::Helpers::Serializer::DeserializeVec3(p_doc, p_node, "custom_bounding_sphere_position", m_customBoundingSphere.position);
	VisionCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "custom_bounding_sphere_radius", m_customBoundingSphere.radius);
}

void VisionCore::ECS::Components::CModelRenderer::OnInspector(VisionUI::Internal::WidgetContainer& p_root)
{
	using namespace VisionCore::Helpers;

	GUIDrawer::DrawMesh(p_root, "Model", m_model, &m_modelChangedEvent);

	GUIDrawer::CreateTitle(p_root, "Frustum Culling Behaviour");
	auto& boundingMode = p_root.CreateWidget<VisionUI::Widgets::Selection::ComboBox>(0);
	boundingMode.choices.emplace(0, "Disabled");
	boundingMode.choices.emplace(1, "Cull model");
	boundingMode.choices.emplace(2, "Cull model & sub-meshes");
	boundingMode.choices.emplace(3, "Cull custom bounding sphere");
	auto& boundingModeDispatcher = boundingMode.AddPlugin<VisionUI::Plugins::DataDispatcher<int>>();
	boundingModeDispatcher.RegisterReference(reinterpret_cast<int&>(m_frustumBehaviour));

	auto& centerLabel = p_root.CreateWidget<VisionUI::Widgets::Texts::TextColored>("Bounding Sphere Center", GUIDrawer::TitleColor);
	auto& centerWidget = p_root.CreateWidget<VisionUI::Widgets::Drags::DragMultipleScalars<float, 3>>(GUIDrawer::GetDataType<float>(), GUIDrawer::_MIN_FLOAT, GUIDrawer::_MAX_FLOAT, 0.f, 0.05f, "", GUIDrawer::GetFormat<float>());
	auto& centerDispatcher = centerWidget.AddPlugin<VisionUI::Plugins::DataDispatcher<std::array<float, 3>>>();
	centerDispatcher.RegisterReference(reinterpret_cast<std::array<float, 3>&>(m_customBoundingSphere.position));

	auto& radiusLabel = p_root.CreateWidget<VisionUI::Widgets::Texts::TextColored>("Bounding Sphere Radius", GUIDrawer::TitleColor);
	auto& radiusWidget = p_root.CreateWidget<VisionUI::Widgets::Drags::DragFloat>(0.0f, GUIDrawer::_MAX_FLOAT, 0.f, 0.1f);
	auto& radiusDispatcher = radiusWidget.AddPlugin<VisionUI::Plugins::DataDispatcher<float>>();
	radiusDispatcher.RegisterReference(m_customBoundingSphere.radius);

	boundingMode.ValueChangedEvent += [&](int p_choice)
	{
		centerLabel.enabled = centerWidget.enabled = radiusLabel.enabled = radiusWidget.enabled = p_choice == 3;
	};

	centerLabel.enabled = centerWidget.enabled = radiusLabel.enabled = radiusWidget.enabled = m_frustumBehaviour == EFrustumBehaviour::CULL_CUSTOM;
}
