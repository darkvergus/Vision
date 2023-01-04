#include "VisionMaths/FTransform.h"

VisionMaths::FTransform::FTransform(FVector3 p_localPosition, FQuaternion p_localRotation, FVector3 p_localScale) :
	m_notificationHandlerID(-1),
	m_parent(nullptr)
{
	GenerateMatrices(p_localPosition, p_localRotation, p_localScale);
}

VisionMaths::FTransform::~FTransform()
{
	Notifier.NotifyChildren(Internal::TransformNotifier::ENotification::TRANSFORM_DESTROYED);
}

void VisionMaths::FTransform::NotificationHandler(Internal::TransformNotifier::ENotification p_notification)
{
	switch (p_notification)
	{
	case Internal::TransformNotifier::ENotification::TRANSFORM_CHANGED:
		UpdateWorldMatrix();
		break;

	case Internal::TransformNotifier::ENotification::TRANSFORM_DESTROYED:
		/* 
		* RemoveParent() is not called here because it is unsafe to remove a notification handler
		* while the parent is iterating on his notification handlers (Segfault otherwise)
		*/
		GenerateMatrices(m_worldPosition, m_worldRotation, m_worldScale);
		m_parent = nullptr;
		UpdateWorldMatrix();
		break;
	}
}

void VisionMaths::FTransform::SetParent(FTransform& p_parent)
{
	m_parent = &p_parent;

	m_notificationHandlerID = m_parent->Notifier.AddNotificationHandler(std::bind(&FTransform::NotificationHandler, this, std::placeholders::_1));

	UpdateWorldMatrix();
}

bool VisionMaths::FTransform::RemoveParent()
{
	if (m_parent != nullptr)
	{
		m_parent->Notifier.RemoveNotificationHandler(m_notificationHandlerID);
		m_parent = nullptr;
		UpdateWorldMatrix();

		return true;
	}

	return false;
}

bool VisionMaths::FTransform::HasParent() const
{
	return m_parent != nullptr;
}

void VisionMaths::FTransform::GenerateMatrices(FVector3 p_position, FQuaternion p_rotation, FVector3 p_scale)
{
	m_localMatrix = FMatrix4::Translation(p_position) * FQuaternion::ToMatrix4(FQuaternion::Normalize(p_rotation)) * FMatrix4::Scaling(p_scale);
	m_localPosition = p_position;
	m_localRotation = p_rotation;
	m_localScale = p_scale;

	UpdateWorldMatrix();
}

void VisionMaths::FTransform::UpdateWorldMatrix()
{
	m_worldMatrix = HasParent() ? m_parent->m_worldMatrix * m_localMatrix : m_localMatrix;
	PreDecomposeWorldMatrix();

	Notifier.NotifyChildren(Internal::TransformNotifier::ENotification::TRANSFORM_CHANGED);
}

void VisionMaths::FTransform::SetLocalPosition(FVector3 p_newPosition)
{
	GenerateMatrices(p_newPosition, m_localRotation, m_localScale);
}

void VisionMaths::FTransform::SetLocalRotation(FQuaternion p_newRotation)
{
	GenerateMatrices(m_localPosition, p_newRotation, m_localScale);
}

void VisionMaths::FTransform::SetLocalScale(FVector3 p_newScale)
{
	GenerateMatrices(m_localPosition, m_localRotation, p_newScale);
}

void VisionMaths::FTransform::SetWorldPosition(FVector3 p_newPosition)
{
	GenerateMatrices(p_newPosition, m_worldRotation, m_worldScale);
}

void VisionMaths::FTransform::SetWorldRotation(FQuaternion p_newRotation)
{
	GenerateMatrices(m_worldPosition, p_newRotation, m_worldScale);
}

void VisionMaths::FTransform::SetWorldScale(FVector3 p_newScale)
{
	GenerateMatrices(m_worldPosition, m_worldRotation, p_newScale);
}

void VisionMaths::FTransform::TranslateLocal(const FVector3& p_translation)
{
	SetLocalPosition(m_localPosition + p_translation);
}

void VisionMaths::FTransform::RotateLocal(const FQuaternion& p_rotation)
{
	SetLocalRotation(m_localRotation * p_rotation);
}

void VisionMaths::FTransform::ScaleLocal(const FVector3& p_scale)
{
	SetLocalScale(FVector3
	(
		m_localScale.x * p_scale.x,
		m_localScale.y * p_scale.y,
		m_localScale.z * p_scale.z
	));
}

const VisionMaths::FVector3& VisionMaths::FTransform::GetLocalPosition() const
{
	return m_localPosition;
}

const VisionMaths::FQuaternion& VisionMaths::FTransform::GetLocalRotation() const
{
	return m_localRotation;
}

const VisionMaths::FVector3& VisionMaths::FTransform::GetLocalScale() const
{
	return m_localScale;
}

const VisionMaths::FVector3& VisionMaths::FTransform::GetWorldPosition() const
{
	return m_worldPosition;
}

const VisionMaths::FQuaternion& VisionMaths::FTransform::GetWorldRotation() const
{
	return m_worldRotation;
}

const VisionMaths::FVector3& VisionMaths::FTransform::GetWorldScale() const
{
	return m_worldScale;
}

const VisionMaths::FMatrix4& VisionMaths::FTransform::GetLocalMatrix() const
{
	return m_localMatrix;
}

const VisionMaths::FMatrix4& VisionMaths::FTransform::GetWorldMatrix() const
{
	return m_worldMatrix;
}

VisionMaths::FVector3 VisionMaths::FTransform::GetWorldForward() const
{
	return m_worldRotation * FVector3::Forward;
}

VisionMaths::FVector3 VisionMaths::FTransform::GetWorldUp() const
{
	return m_worldRotation * FVector3::Up;
}

VisionMaths::FVector3 VisionMaths::FTransform::GetWorldRight() const
{
	return m_worldRotation * FVector3::Right;
}

VisionMaths::FVector3 VisionMaths::FTransform::GetLocalForward() const
{
	return m_localRotation * FVector3::Forward;
}

VisionMaths::FVector3 VisionMaths::FTransform::GetLocalUp() const
{
	return m_localRotation * FVector3::Up;
}

VisionMaths::FVector3 VisionMaths::FTransform::GetLocalRight() const
{
	return m_localRotation * FVector3::Right;
}

void VisionMaths::FTransform::PreDecomposeWorldMatrix()
{
	m_worldPosition.x = m_worldMatrix(0, 3);
	m_worldPosition.y = m_worldMatrix(1, 3);
	m_worldPosition.z = m_worldMatrix(2, 3);

	FVector3 columns[3] = 
	{
		{ m_worldMatrix(0, 0), m_worldMatrix(1, 0), m_worldMatrix(2, 0)},
		{ m_worldMatrix(0, 1), m_worldMatrix(1, 1), m_worldMatrix(2, 1)},
		{ m_worldMatrix(0, 2), m_worldMatrix(1, 2), m_worldMatrix(2, 2)},
	};

	m_worldScale.x = FVector3::Length(columns[0]);
	m_worldScale.y = FVector3::Length(columns[1]);
	m_worldScale.z = FVector3::Length(columns[2]);

	if (m_worldScale.x)
	{
		columns[0] /= m_worldScale.x;
	}
	if (m_worldScale.y)
	{
		columns[1] /= m_worldScale.y;
	}
	if (m_worldScale.z)
	{
		columns[2] /= m_worldScale.z;
	}

	FMatrix3 rotationMatrix
	(
		columns[0].x, columns[1].x, columns[2].x,
		columns[0].y, columns[1].y, columns[2].y,
		columns[0].z, columns[1].z, columns[2].z
	);

	m_worldRotation = FQuaternion(rotationMatrix);
}

void VisionMaths::FTransform::PreDecomposeLocalMatrix()
{
	m_localPosition.x = m_localMatrix(0, 3);
	m_localPosition.y = m_localMatrix(1, 3);
	m_localPosition.z = m_localMatrix(2, 3);

	FVector3 columns[3] =
	{
		{ m_localMatrix(0, 0), m_localMatrix(1, 0), m_localMatrix(2, 0)},
		{ m_localMatrix(0, 1), m_localMatrix(1, 1), m_localMatrix(2, 1)},
		{ m_localMatrix(0, 2), m_localMatrix(1, 2), m_localMatrix(2, 2)},
	};

	m_localScale.x = FVector3::Length(columns[0]);
	m_localScale.y = FVector3::Length(columns[1]);
	m_localScale.z = FVector3::Length(columns[2]);

	if (m_localScale.x)
	{
		columns[0] /= m_localScale.x;
	}
	if (m_localScale.y)
	{
		columns[1] /= m_localScale.y;
	}
	if (m_localScale.z)
	{
		columns[2] /= m_localScale.z;
	}

	FMatrix3 rotationMatrix
	(
		columns[0].x, columns[1].x, columns[2].x,
		columns[0].y, columns[1].y, columns[2].y,
		columns[0].z, columns[1].z, columns[2].z
	);

	m_localRotation = FQuaternion(rotationMatrix);
}