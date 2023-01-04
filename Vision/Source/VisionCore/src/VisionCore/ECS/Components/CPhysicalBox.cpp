#include <VisionPhysics/Entities/PhysicalBox.h>

#include "VisionCore/ECS/Components/CPhysicalBox.h"
#include "VisionCore/ECS/Actor.h"

using namespace VisionPhysics::Entities;

VisionCore::ECS::Components::CPhysicalBox::CPhysicalBox(ECS::Actor & p_owner) :
	CPhysicalObject(p_owner)
{
	m_physicalObject = std::make_unique<VisionPhysics::Entities::PhysicalBox>(p_owner.transform.GetFTransform());

	m_physicalObject->SetUserData<std::reference_wrapper<CPhysicalObject>>(*this);

	BindListener();
	Init();
}

std::string VisionCore::ECS::Components::CPhysicalBox::GetName()
{
	return "Physical Box";
}

void VisionCore::ECS::Components::CPhysicalBox::SetSize(const VisionMaths::FVector3 & p_size)
{
	GetPhysicalObjectAs<PhysicalBox>().SetSize(p_size);
}

VisionMaths::FVector3 VisionCore::ECS::Components::CPhysicalBox::GetSize() const
{
	return GetPhysicalObjectAs<PhysicalBox>().GetSize();
}

void VisionCore::ECS::Components::CPhysicalBox::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CPhysicalObject::OnSerialize(p_doc, p_node);

	Helpers::Serializer::SerializeVec3(p_doc, p_node, "size", GetSize());
}

void VisionCore::ECS::Components::CPhysicalBox::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CPhysicalObject::OnDeserialize(p_doc, p_node);

	SetSize(Helpers::Serializer::DeserializeVec3(p_doc, p_node, "size"));
}

void VisionCore::ECS::Components::CPhysicalBox::OnInspector(VisionUI::Internal::WidgetContainer & p_root)
{
	CPhysicalObject::OnInspector(p_root);

	Helpers::GUIDrawer::DrawVec3(p_root, "Size", std::bind(&CPhysicalBox::GetSize, this), std::bind(&CPhysicalBox::SetSize, this, std::placeholders::_1), 0.1f, 0.f, 100000.f);
}