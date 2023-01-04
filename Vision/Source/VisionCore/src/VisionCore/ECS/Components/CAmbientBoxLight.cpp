#include <VisionUI/Widgets/Texts/Text.h>
#include <VisionUI/Widgets/Drags/DragFloat.h>
#include <VisionUI/Widgets/Selection/ColorEdit.h>
#include <VisionUI/Widgets/Buttons/Button.h>

#include "VisionCore/ECS/Actor.h"

#include "VisionCore/ECS/Components/CAmbientBoxLight.h"

VisionCore::ECS::Components::CAmbientBoxLight::CAmbientBoxLight(ECS::Actor & p_owner) :
	CLight(p_owner)
{
	m_data.type = static_cast<float>(VisionRendering::Entities::Light::Type::AMBIENT_BOX);

	m_data.intensity = 0.1f;
	m_data.constant = 1.0f;
	m_data.linear = 1.0f;
	m_data.quadratic = 1.0f;
}

std::string VisionCore::ECS::Components::CAmbientBoxLight::GetName()
{
	return "Ambient Box Light";
}

VisionMaths::FVector3 VisionCore::ECS::Components::CAmbientBoxLight::GetSize() const
{
	return { m_data.constant, m_data.linear, m_data.quadratic };
}

void VisionCore::ECS::Components::CAmbientBoxLight::SetSize(const VisionMaths::FVector3& p_size)
{
	m_data.constant = p_size.x;
	m_data.linear = p_size.y;
	m_data.quadratic = p_size.z;
}

void VisionCore::ECS::Components::CAmbientBoxLight::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	using namespace VisionCore::Helpers;

	CLight::OnSerialize(p_doc, p_node);

	Serializer::SerializeVec3(p_doc, p_node, "size", { m_data.constant, m_data.linear, m_data.quadratic });
}

void VisionCore::ECS::Components::CAmbientBoxLight::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	using namespace VisionCore::Helpers;

	CLight::OnDeserialize(p_doc, p_node);

	VisionMaths::FVector3 size = Serializer::DeserializeVec3(p_doc, p_node, "size");
	m_data.constant = size.x;
	m_data.linear = size.y;
	m_data.quadratic = size.z;
}

void VisionCore::ECS::Components::CAmbientBoxLight::OnInspector(VisionUI::Internal::WidgetContainer& p_root)
{
	using namespace VisionCore::Helpers;

	CLight::OnInspector(p_root);

	auto sizeGatherer = [this]() -> VisionMaths::FVector3 { return { m_data.constant, m_data.linear, m_data.quadratic }; };
	auto sizeProvider = [this](const VisionMaths::FVector3& p_data) { m_data.constant = p_data.x; m_data.linear = p_data.y, m_data.quadratic = p_data.z; };

	GUIDrawer::DrawVec3(p_root, "Size", sizeGatherer, sizeProvider, 0.1f, 0.f);
}
