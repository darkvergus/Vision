#include <VisionUI/Widgets/Texts/Text.h>
#include <VisionUI/Widgets/Drags/DragFloat.h>
#include <VisionUI/Widgets/Selection/ColorEdit.h>
#include <VisionUI/Widgets/Buttons/Button.h>
#include <VisionUI/Widgets/Layout/Group.h>

#include "VisionCore/ECS/Actor.h"

#include "VisionCore/ECS/Components/CPointLight.h"

VisionCore::ECS::Components::CPointLight::CPointLight(ECS::Actor& p_owner) :
	CLight(p_owner)
{
	m_data.type = static_cast<float>(VisionRendering::Entities::Light::Type::POINT);
}

std::string VisionCore::ECS::Components::CPointLight::GetName()
{
	return "Point Light";
}

float VisionCore::ECS::Components::CPointLight::GetConstant() const
{
	return m_data.constant;
}

float VisionCore::ECS::Components::CPointLight::GetLinear() const
{
	return m_data.linear;
}

float VisionCore::ECS::Components::CPointLight::GetQuadratic() const
{
	return m_data.quadratic;
}

void VisionCore::ECS::Components::CPointLight::SetConstant(float p_constant)
{
	m_data.constant = p_constant;
}

void VisionCore::ECS::Components::CPointLight::SetLinear(float p_linear)
{
	m_data.linear = p_linear;
}

void VisionCore::ECS::Components::CPointLight::SetQuadratic(float p_quadratic)
{
	m_data.quadratic = p_quadratic;
}

void VisionCore::ECS::Components::CPointLight::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	using namespace VisionCore::Helpers;

	CLight::OnSerialize(p_doc, p_node);

	Serializer::SerializeFloat(p_doc, p_node, "constant", m_data.constant);
	Serializer::SerializeFloat(p_doc, p_node, "linear", m_data.linear);
	Serializer::SerializeFloat(p_doc, p_node, "quadratic", m_data.quadratic);
}

void VisionCore::ECS::Components::CPointLight::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	using namespace VisionCore::Helpers;

	CLight::OnDeserialize(p_doc, p_node);

	Serializer::DeserializeFloat(p_doc, p_node, "constant", m_data.constant);
	Serializer::DeserializeFloat(p_doc, p_node, "linear", m_data.linear);
	Serializer::DeserializeFloat(p_doc, p_node, "quadratic", m_data.quadratic);
}

void VisionCore::ECS::Components::CPointLight::OnInspector(VisionUI::Internal::WidgetContainer& p_root)
{
	using namespace VisionCore::Helpers;

	CLight::OnInspector(p_root);

	GUIDrawer::CreateTitle(p_root, "Fall-off presets");
	auto& presetsRoot = p_root.CreateWidget<VisionUI::Widgets::Layout::Group>();

	auto& constantPreset = presetsRoot.CreateWidget<VisionUI::Widgets::Buttons::Button>("Constant");
	constantPreset.ClickedEvent += [this] { m_data.constant = 1.f, m_data.linear = m_data.quadratic = 0.f; };
	constantPreset.lineBreak = false;
	constantPreset.idleBackgroundColor = { 0.7f, 0.5f, 0.f };

	auto& linearPreset = presetsRoot.CreateWidget<VisionUI::Widgets::Buttons::Button>("Linear");
	linearPreset.ClickedEvent += [this] { m_data.linear = 1.f, m_data.constant = m_data.quadratic = 0.f; };
	linearPreset.lineBreak = false;
	linearPreset.idleBackgroundColor = { 0.7f, 0.5f, 0.f };

	auto& quadraticPreset = presetsRoot.CreateWidget<VisionUI::Widgets::Buttons::Button>("Quadratic");
	quadraticPreset.ClickedEvent += [this] { m_data.quadratic = 1.f, m_data.constant = m_data.linear = 0.f; };
	quadraticPreset.idleBackgroundColor = { 0.7f, 0.5f, 0.f };

	GUIDrawer::DrawScalar<float>(p_root, "Constant", m_data.constant, 0.005f, 0.f);
	GUIDrawer::DrawScalar<float>(p_root, "Linear", m_data.linear, 0.005f, 0.f);
	GUIDrawer::DrawScalar<float>(p_root, "Quadratic", m_data.quadratic, 0.005f, 0.f);
}
