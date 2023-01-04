#include <algorithm>

#include "VisionPhysics/Entities/PhysicalSphere.h"

VisionPhysics::Entities::PhysicalSphere::PhysicalSphere(float p_radius) : PhysicalObject()
{
	CreateCollisionShape(p_radius);
	Init();
}

VisionPhysics::Entities::PhysicalSphere::PhysicalSphere(VisionMaths::FTransform & p_transform, float p_radius) : PhysicalObject(p_transform)
{
	CreateCollisionShape(p_radius);
	Init();
}

void VisionPhysics::Entities::PhysicalSphere::SetRadius(float p_radius)
{
	if (p_radius != m_radius)
		RecreateCollisionShape(p_radius);
}

float VisionPhysics::Entities::PhysicalSphere::GetRadius() const
{
	return m_radius;
}


void VisionPhysics::Entities::PhysicalSphere::CreateCollisionShape(float p_radius)
{
	m_shape = std::make_unique<btSphereShape>(p_radius);
	m_radius = p_radius;
}

void VisionPhysics::Entities::PhysicalSphere::RecreateCollisionShape(float p_radius)
{
	CreateCollisionShape(p_radius);
	RecreateBody();
}

void VisionPhysics::Entities::PhysicalSphere::SetLocalScaling(const VisionMaths::FVector3 & p_scaling)
{
	float radiusScale = std::max(std::max(p_scaling.x, p_scaling.y), p_scaling.z);
	m_shape->setLocalScaling({ radiusScale, radiusScale, radiusScale });
}
