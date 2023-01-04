#include <VisionUI/Widgets/Texts/Text.h>
#include <VisionUI/Widgets/Drags/DragFloat.h>
#include <VisionUI/Widgets/Selection/ColorEdit.h>

#include "VisionCore/ECS/Actor.h"

#include "VisionCore/ECS/Components/CDirectionalLight.h"

VisionCore::ECS::Components::CDirectionalLight::CDirectionalLight(ECS::Actor & p_owner) :
	CLight(p_owner)
{
	m_data.type = static_cast<float>(VisionRendering::Entities::Light::Type::DIRECTIONAL);
}

std::string VisionCore::ECS::Components::CDirectionalLight::GetName()
{
	return "Directional Light";
}

void VisionCore::ECS::Components::CDirectionalLight::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CLight::OnSerialize(p_doc, p_node);
}

void VisionCore::ECS::Components::CDirectionalLight::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CLight::OnDeserialize(p_doc, p_node);
}

void VisionCore::ECS::Components::CDirectionalLight::OnInspector(VisionUI::Internal::WidgetContainer& p_root)
{
	CLight::OnInspector(p_root);
}
