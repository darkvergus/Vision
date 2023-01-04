#include <VisionUI/Widgets/Texts/Text.h>
#include <VisionUI/Widgets/Drags/DragFloat.h>
#include <VisionUI/Widgets/Selection/ColorEdit.h>
#include <VisionUI/Widgets/Buttons/Button.h>
#include <VisionUI/Widgets/Layout/Group.h>

#include "VisionCore/ECS/Actor.h"

#include "VisionCore/ECS/Components/CLight.h"

VisionCore::ECS::Components::CLight::CLight(ECS::Actor & p_owner) :
	AComponent(p_owner),
	m_data(p_owner.transform.GetFTransform(), {})
{
}

const VisionRendering::Entities::Light& VisionCore::ECS::Components::CLight::GetData() const
{
	return m_data;
}

const VisionMaths::FVector3& VisionCore::ECS::Components::CLight::GetColor() const
{
	return m_data.color;
}

float VisionCore::ECS::Components::CLight::GetIntensity() const
{
	return m_data.intensity;
}

void VisionCore::ECS::Components::CLight::SetColor(const VisionMaths::FVector3& p_color)
{
	m_data.color = p_color;
}

void VisionCore::ECS::Components::CLight::SetIntensity(float p_intensity)
{
	m_data.intensity = p_intensity;
}

void VisionCore::ECS::Components::CLight::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	using namespace VisionCore::Helpers;

	Serializer::SerializeVec3(p_doc, p_node, "color", m_data.color);
	Serializer::SerializeFloat(p_doc, p_node, "intensity", m_data.intensity);
}

void VisionCore::ECS::Components::CLight::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	using namespace VisionCore::Helpers;

	Serializer::DeserializeVec3(p_doc, p_node, "color", m_data.color);
	Serializer::DeserializeFloat(p_doc, p_node, "intensity", m_data.intensity);
}

void VisionCore::ECS::Components::CLight::OnInspector(VisionUI::Internal::WidgetContainer& p_root)
{
	using namespace VisionCore::Helpers;

	GUIDrawer::DrawColor(p_root, "Color", reinterpret_cast<VisionUI::Types::Color&>(m_data.color));
	GUIDrawer::DrawScalar<float>(p_root, "Intensity", m_data.intensity, 0.005f, GUIDrawer::_MIN_FLOAT, GUIDrawer::_MAX_FLOAT);
}
