#include <VisionUI/Widgets/Texts/Text.h>
#include <VisionUI/Widgets/Drags/DragFloat.h>
#include <VisionUI/Widgets/Selection/ColorEdit.h>
#include <VisionUI/Widgets/Buttons/Button.h>

#include "VisionCore/ECS/Actor.h"

#include "VisionCore/ECS/Components/CAmbientSphereLight.h"

VisionCore::ECS::Components::CAmbientSphereLight::CAmbientSphereLight(ECS::Actor & p_owner) :
	CLight(p_owner)
{
    m_data.intensity = 0.1f;
	m_data.constant = 1.0f;

	m_data.type = static_cast<float>(VisionRendering::Entities::Light::Type::AMBIENT_SPHERE);
}

std::string VisionCore::ECS::Components::CAmbientSphereLight::GetName()
{
	return "Ambient Sphere Light";
}

float VisionCore::ECS::Components::CAmbientSphereLight::GetRadius() const
{
	return m_data.quadratic;
}

void VisionCore::ECS::Components::CAmbientSphereLight::SetRadius(float p_radius)
{
	m_data.constant = p_radius;
}

void VisionCore::ECS::Components::CAmbientSphereLight::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	using namespace VisionCore::Helpers;

	CLight::OnSerialize(p_doc, p_node);

	Serializer::SerializeFloat(p_doc, p_node, "radius", m_data.constant);
}

void VisionCore::ECS::Components::CAmbientSphereLight::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	using namespace VisionCore::Helpers;

	CLight::OnDeserialize(p_doc, p_node);

	Serializer::DeserializeFloat(p_doc, p_node, "radius", m_data.constant);
}

void VisionCore::ECS::Components::CAmbientSphereLight::OnInspector(VisionUI::Internal::WidgetContainer& p_root)
{
	using namespace VisionCore::Helpers;

	CLight::OnInspector(p_root);

	GUIDrawer::DrawScalar<float>(p_root, "Radius", m_data.constant, 0.1f, 0.f);
}
