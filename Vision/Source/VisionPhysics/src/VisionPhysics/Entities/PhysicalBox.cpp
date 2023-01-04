#include "VisionPhysics/Entities/PhysicalBox.h"
#include "VisionPhysics/Tools/Conversion.h"

VisionPhysics::Entities::PhysicalBox::PhysicalBox(const VisionMaths::FVector3& p_size) : PhysicalObject()
{
	CreateCollisionShape(p_size);
	Init();
}

VisionPhysics::Entities::PhysicalBox::PhysicalBox(VisionMaths::FTransform & p_transform, const VisionMaths::FVector3& p_size) : PhysicalObject(p_transform)
{
	CreateCollisionShape(p_size);
	Init();
}

void VisionPhysics::Entities::PhysicalBox::SetSize(const VisionMaths::FVector3& p_size)
{
	if (m_size != p_size)
		RecreateCollisionShape(p_size);
}

VisionMaths::FVector3 VisionPhysics::Entities::PhysicalBox::GetSize() const
{
	return m_size;
}


void VisionPhysics::Entities::PhysicalBox::CreateCollisionShape(const VisionMaths::FVector3& p_size)
{
	m_shape = std::make_unique<btBoxShape>(VisionPhysics::Tools::Conversion::ToBtVector3(p_size));
	m_size = p_size;
}

void VisionPhysics::Entities::PhysicalBox::RecreateCollisionShape(const VisionMaths::FVector3& p_size)
{
	CreateCollisionShape(p_size);
	RecreateBody();
}

void VisionPhysics::Entities::PhysicalBox::SetLocalScaling(const VisionMaths::FVector3 & p_scaling)
{
	m_shape->setLocalScaling(VisionPhysics::Tools::Conversion::ToBtVector3(p_scaling));
}