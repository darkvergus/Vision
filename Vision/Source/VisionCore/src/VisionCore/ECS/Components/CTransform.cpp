#include <VisionUI/Widgets/Texts/Text.h>
#include <VisionUI/Widgets/Drags/DragMultipleFloats.h>

#include "VisionCore/ECS/Components/CTransform.h"

VisionCore::ECS::Components::CTransform::CTransform(ECS::Actor& p_owner, VisionMaths::FVector3 p_localPosition, VisionMaths::FQuaternion p_localRotation, VisionMaths::FVector3 p_localScale) :
AComponent(p_owner)
{
	m_transform.GenerateMatrices(p_localPosition, p_localRotation, p_localScale);
}

std::string VisionCore::ECS::Components::CTransform::GetName()
{
	return "Transform";
}

void VisionCore::ECS::Components::CTransform::SetParent(CTransform& p_parent)
{
	m_transform.SetParent(p_parent.GetFTransform());
}

bool VisionCore::ECS::Components::CTransform::RemoveParent()
{
	return m_transform.RemoveParent();
}

bool VisionCore::ECS::Components::CTransform::HasParent() const
{
	return m_transform.HasParent();
}

void VisionCore::ECS::Components::CTransform::SetLocalPosition(VisionMaths::FVector3 p_newPosition)
{
	m_transform.SetLocalPosition(p_newPosition);
}

void VisionCore::ECS::Components::CTransform::SetLocalRotation(VisionMaths::FQuaternion p_newRotation)
{
	m_transform.SetLocalRotation(p_newRotation);
}

void VisionCore::ECS::Components::CTransform::SetLocalScale(VisionMaths::FVector3 p_newScale)
{
	m_transform.SetLocalScale(p_newScale);
}

void VisionCore::ECS::Components::CTransform::SetWorldPosition(VisionMaths::FVector3 p_newPosition)
{
	m_transform.SetWorldPosition(p_newPosition);
}

void VisionCore::ECS::Components::CTransform::SetWorldRotation(VisionMaths::FQuaternion p_newRotation)
{
	m_transform.SetWorldRotation(p_newRotation);
}

void VisionCore::ECS::Components::CTransform::SetWorldScale(VisionMaths::FVector3 p_newScale)
{
	m_transform.SetWorldScale(p_newScale);
}

void VisionCore::ECS::Components::CTransform::TranslateLocal(const VisionMaths::FVector3 & p_translation)
{
	m_transform.TranslateLocal(p_translation);
}

void VisionCore::ECS::Components::CTransform::RotateLocal(const VisionMaths::FQuaternion & p_rotation)
{
	m_transform.RotateLocal(p_rotation);
}

void VisionCore::ECS::Components::CTransform::ScaleLocal(const VisionMaths::FVector3 & p_scale)
{
	m_transform.ScaleLocal(p_scale);
}

const VisionMaths::FVector3 & VisionCore::ECS::Components::CTransform::GetLocalPosition() const
{
	return m_transform.GetLocalPosition();
}

const VisionMaths::FQuaternion & VisionCore::ECS::Components::CTransform::GetLocalRotation() const
{
	return m_transform.GetLocalRotation();
}

const VisionMaths::FVector3 & VisionCore::ECS::Components::CTransform::GetLocalScale() const
{
	return m_transform.GetLocalScale();
}

const VisionMaths::FVector3 & VisionCore::ECS::Components::CTransform::GetWorldPosition() const
{
	return m_transform.GetWorldPosition();
}

const VisionMaths::FQuaternion & VisionCore::ECS::Components::CTransform::GetWorldRotation() const
{
	return m_transform.GetWorldRotation();
}

const VisionMaths::FVector3 & VisionCore::ECS::Components::CTransform::GetWorldScale() const
{
	return m_transform.GetWorldScale();
}

const VisionMaths::FMatrix4 & VisionCore::ECS::Components::CTransform::GetLocalMatrix() const
{
	return m_transform.GetLocalMatrix();
}

const VisionMaths::FMatrix4 & VisionCore::ECS::Components::CTransform::GetWorldMatrix() const
{
	return m_transform.GetWorldMatrix();
}

VisionMaths::FTransform & VisionCore::ECS::Components::CTransform::GetFTransform()
{
	return m_transform;
}

VisionMaths::FVector3 VisionCore::ECS::Components::CTransform::GetWorldForward() const
{
	return m_transform.GetWorldForward();
}

VisionMaths::FVector3 VisionCore::ECS::Components::CTransform::GetWorldUp() const
{
	return m_transform.GetWorldUp();
}

VisionMaths::FVector3 VisionCore::ECS::Components::CTransform::GetWorldRight() const
{
	return m_transform.GetWorldRight();
}

VisionMaths::FVector3 VisionCore::ECS::Components::CTransform::GetLocalForward() const
{
	return m_transform.GetLocalForward();
}

VisionMaths::FVector3 VisionCore::ECS::Components::CTransform::GetLocalUp() const
{
	return m_transform.GetLocalUp();
}

VisionMaths::FVector3 VisionCore::ECS::Components::CTransform::GetLocalRight() const
{
	return m_transform.GetLocalRight();
}

void VisionCore::ECS::Components::CTransform::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	VisionCore::Helpers::Serializer::SerializeVec3(p_doc, p_node, "position", GetLocalPosition());
	VisionCore::Helpers::Serializer::SerializeQuat(p_doc, p_node, "rotation", GetLocalRotation());
	VisionCore::Helpers::Serializer::SerializeVec3(p_doc, p_node, "scale", GetLocalScale());
}

void VisionCore::ECS::Components::CTransform::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	m_transform.GenerateMatrices
	(
		VisionCore::Helpers::Serializer::DeserializeVec3(p_doc, p_node, "position"),
		VisionCore::Helpers::Serializer::DeserializeQuat(p_doc, p_node, "rotation"),
		VisionCore::Helpers::Serializer::DeserializeVec3(p_doc, p_node, "scale")
	);
}

void VisionCore::ECS::Components::CTransform::OnInspector(VisionUI::Internal::WidgetContainer& p_root)
{
	auto getRotation = [this]
	{ 
		return VisionMaths::FQuaternion::EulerAngles(GetLocalRotation());
	};

	auto setRotation = [this](VisionMaths::FVector3 result)
	{
		SetLocalRotation(VisionMaths::FQuaternion(result));
	};

	VisionCore::Helpers::GUIDrawer::DrawVec3(p_root, "Position", std::bind(&CTransform::GetLocalPosition, this), std::bind(&CTransform::SetLocalPosition, this, std::placeholders::_1), 0.05f);
	VisionCore::Helpers::GUIDrawer::DrawVec3(p_root, "Rotation", getRotation, setRotation, 0.05f);
	VisionCore::Helpers::GUIDrawer::DrawVec3(p_root, "Scale", std::bind(&CTransform::GetLocalScale, this), std::bind(&CTransform::SetLocalScale, this, std::placeholders::_1), 0.05f, 0.0001f);
}
