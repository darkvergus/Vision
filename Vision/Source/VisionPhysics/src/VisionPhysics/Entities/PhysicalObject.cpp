#include "VisionPhysics/Entities/PhysicalObject.h"

#include "VisionPhysics/Tools/Conversion.h"
#include "VisionDebug/Logger.h"

using namespace VisionPhysics::Tools;
using namespace VisionPhysics::Settings;

VisionTools::Eventing::Event<VisionPhysics::Entities::PhysicalObject&>	VisionPhysics::Entities::PhysicalObject::CreatedEvent;
VisionTools::Eventing::Event<VisionPhysics::Entities::PhysicalObject&>	VisionPhysics::Entities::PhysicalObject::DestroyedEvent;
VisionTools::Eventing::Event<btRigidBody&>							VisionPhysics::Entities::PhysicalObject::ConsiderEvent;
VisionTools::Eventing::Event<btRigidBody&>							VisionPhysics::Entities::PhysicalObject::UnconsiderEvent;

VisionPhysics::Entities::PhysicalObject::PhysicalObject() :
	m_transform(new VisionMaths::FTransform()),
	m_internalTransform(true)
{
	CollisionStartEvent += [this](VisionPhysics::Entities::PhysicalObject& otherPhysicalObject)
	{
		UpdateBtTransform();
	};
}

VisionPhysics::Entities::PhysicalObject::PhysicalObject(VisionMaths::FTransform& p_transform) :
	m_transform(&p_transform),
	m_internalTransform(false)
{

}

VisionPhysics::Entities::PhysicalObject::~PhysicalObject()
{
	DestroyBody();
	PhysicalObject::DestroyedEvent.Invoke(*this);

	if (m_internalTransform)
		delete m_transform;
}

void VisionPhysics::Entities::PhysicalObject::Init()
{
	PhysicalObject::CreatedEvent.Invoke(*this);
	CreateBody({});
}

void VisionPhysics::Entities::PhysicalObject::AddForce(const VisionMaths::FVector3& p_force)
{
	m_body->applyCentralForce(Conversion::ToBtVector3(p_force));
}

void VisionPhysics::Entities::PhysicalObject::AddImpulse(const VisionMaths::FVector3& p_impulse)
{
	m_body->applyCentralImpulse(Conversion::ToBtVector3(p_impulse));
}

void VisionPhysics::Entities::PhysicalObject::ClearForces()
{
	m_body->clearForces();
}

void VisionPhysics::Entities::PhysicalObject::AddFlag(btCollisionObject::CollisionFlags p_flag)
{
	m_body->setCollisionFlags(m_body->getCollisionFlags() | p_flag);
}

void VisionPhysics::Entities::PhysicalObject::RemoveFlag(btCollisionObject::CollisionFlags p_flag)
{
	m_body->setCollisionFlags(m_body->getCollisionFlags() & ~p_flag);
}

float VisionPhysics::Entities::PhysicalObject::GetMass() const
{
	return m_mass;
}

const VisionPhysics::Entities::PhysicalObject::ECollisionDetectionMode& VisionPhysics::Entities::PhysicalObject::GetCollisionDetectionMode() const
{
	return m_collisionMode;
}

float VisionPhysics::Entities::PhysicalObject::GetBounciness() const
{
	return m_body->getRestitution();
}

float VisionPhysics::Entities::PhysicalObject::GetFriction() const
{
	return m_body->getFriction();
}

VisionMaths::FVector3 VisionPhysics::Entities::PhysicalObject::GetLinearVelocity() const
{
	return Conversion::ToOvVector3(m_body->getLinearVelocity());
}

VisionMaths::FVector3 VisionPhysics::Entities::PhysicalObject::GetAngularVelocity() const
{
	return Conversion::ToOvVector3(m_body->getAngularVelocity());
}

VisionMaths::FVector3 VisionPhysics::Entities::PhysicalObject::GetLinearFactor() const
{
	return Conversion::ToOvVector3(m_body->getLinearFactor());
}

VisionMaths::FVector3 VisionPhysics::Entities::PhysicalObject::GetAngularFactor() const
{
	return Conversion::ToOvVector3(m_body->getAngularFactor());
}

bool VisionPhysics::Entities::PhysicalObject::IsTrigger() const
{
	return m_trigger;
}

bool VisionPhysics::Entities::PhysicalObject::IsKinematic() const
{
	return m_kinematic;
}

VisionPhysics::Entities::PhysicalObject::EActivationState VisionPhysics::Entities::PhysicalObject::GetActivationState() const
{
	return static_cast<EActivationState>(m_body->getActivationState());
}

VisionMaths::FTransform& VisionPhysics::Entities::PhysicalObject::GetTransform()
{
	return *m_transform;
}

void VisionPhysics::Entities::PhysicalObject::SetMass(float p_mass)
{
	m_mass = p_mass;
	ApplyInertia();
}

void VisionPhysics::Entities::PhysicalObject::SetCollisionDetectionMode(ECollisionDetectionMode p_mode)
{
	m_collisionMode = p_mode;

	switch (m_collisionMode)
	{
	case ECollisionDetectionMode::DISCRETE:
		m_body->setCcdMotionThreshold(std::numeric_limits<float>::max());
		m_body->setCcdSweptSphereRadius(0.0f);
		break;
	case ECollisionDetectionMode::CONTINUOUS:
		m_body->setCcdMotionThreshold(static_cast<btScalar>(1e-7));
		m_body->setCcdSweptSphereRadius(0.5f);
		break;
	}
}

void VisionPhysics::Entities::PhysicalObject::SetBounciness(float p_bounciness)
{
	m_body->setRestitution(p_bounciness);
}

void VisionPhysics::Entities::PhysicalObject::SetFriction(float p_friction)
{
	m_body->setFriction(p_friction);
}

void VisionPhysics::Entities::PhysicalObject::SetLinearVelocity(const VisionMaths::FVector3 & p_linearVelocity)
{
	m_body->setLinearVelocity(Conversion::ToBtVector3(p_linearVelocity));
}

void VisionPhysics::Entities::PhysicalObject::SetAngularVelocity(const VisionMaths::FVector3 & p_angularVelocity)
{
	m_body->setAngularVelocity(Conversion::ToBtVector3(p_angularVelocity));
}

void VisionPhysics::Entities::PhysicalObject::SetLinearFactor(const VisionMaths::FVector3 & p_linearFactor)
{
	m_body->setLinearFactor(Conversion::ToBtVector3(p_linearFactor));
}

void VisionPhysics::Entities::PhysicalObject::SetAngularFactor(const VisionMaths::FVector3 & p_angularFactor)
{
	m_body->setAngularFactor(Conversion::ToBtVector3(p_angularFactor));
}

void VisionPhysics::Entities::PhysicalObject::SetTrigger(bool p_trigger)
{
	if (p_trigger)
		AddFlag(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	else
		RemoveFlag(btCollisionObject::CF_NO_CONTACT_RESPONSE);

	m_trigger = p_trigger;
}

void VisionPhysics::Entities::PhysicalObject::SetKinematic(bool p_kinematic)
{
	m_kinematic = p_kinematic;

	if (m_kinematic)
	{
		ClearForces();
		SetLinearVelocity(VisionMaths::FVector3::Zero);
		SetAngularVelocity(VisionMaths::FVector3::Zero);
	}

	RecreateBody();
}

void VisionPhysics::Entities::PhysicalObject::SetActivationState(EActivationState p_activationState)
{
	m_body->setActivationState(static_cast<int>(p_activationState));
}

void VisionPhysics::Entities::PhysicalObject::SetEnabled(bool p_enabled)
{
	m_enabled = p_enabled;

	if (!m_enabled)
		Unconsider();
	else
		Consider();
}

bool VisionPhysics::Entities::PhysicalObject::IsEnabled() const
{
	return m_enabled;
}

void VisionPhysics::Entities::PhysicalObject::UpdateBtTransform()
{
	m_body->setWorldTransform(Conversion::ToBtTransform(*m_transform));

	if (VisionMaths::FVector3::Distance(m_transform->GetWorldScale(), m_previousScale) >= 0.01f)
	{
		m_previousScale = m_transform->GetWorldScale();
		SetLocalScaling({ abs(m_previousScale.x), abs(m_previousScale.y), abs(m_previousScale.z) });
		RecreateBody();
	}
}

void VisionPhysics::Entities::PhysicalObject::UpdateFTransform()
{
	if (!m_kinematic)
	{
		const btTransform& result = m_body->getWorldTransform();
		m_transform->SetLocalPosition(Conversion::ToOvVector3(result.getOrigin()));
		m_transform->SetLocalRotation(Conversion::ToOvQuaternion(result.getRotation()));
	}
}

void VisionPhysics::Entities::PhysicalObject::RecreateBody()
{
	CreateBody(DestroyBody());
}

void VisionPhysics::Entities::PhysicalObject::ApplyInertia()
{
	m_body->setMassProps(m_kinematic ? 0.0f : std::max(0.0000001f, m_mass), m_kinematic ? btVector3(0.0f, 0.0f, 0.0f) : CalculateInertia());
}

void VisionPhysics::Entities::PhysicalObject::Consider()
{
	if (!m_considered)
	{
		m_considered = true;
		ConsiderEvent.Invoke(*m_body);
	}
}

void VisionPhysics::Entities::PhysicalObject::Unconsider()
{
	if (m_considered)
	{
		m_considered = false;
		UnconsiderEvent.Invoke(*m_body);
	}
}

void VisionPhysics::Entities::PhysicalObject::CreateBody(const Settings::BodySettings & p_bodySettings)
{
	m_motion = std::make_unique<btDefaultMotionState>(Tools::Conversion::ToBtTransform(*m_transform));

	m_body = std::make_unique<btRigidBody>(btRigidBody::btRigidBodyConstructionInfo{ 0.0f, m_motion.get(), m_shape.get(), btVector3(0.0f, 0.0f, 0.0f) });

	ApplyInertia();

	m_body->setRestitution(p_bodySettings.restitution);
	m_body->setFriction(p_bodySettings.friction);
	m_body->setLinearVelocity(p_bodySettings.linearVelocity);
	m_body->setAngularVelocity(p_bodySettings.angularVelocity);
	m_body->setLinearFactor(p_bodySettings.linearFactor);
	m_body->setAngularFactor(p_bodySettings.angularFactor);
	m_body->setUserPointer(this);

	AddFlag(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

	if (p_bodySettings.isTrigger)
		AddFlag(btCollisionObject::CF_NO_CONTACT_RESPONSE);

	SetActivationState(EActivationState::ALWAYS_ACTIVE); // TODO: Avoid using always active

	if (m_enabled)
		Consider();
}

VisionPhysics::Settings::BodySettings VisionPhysics::Entities::PhysicalObject::DestroyBody()
{
	BodySettings result
	{
		m_body->getLinearVelocity(),
		m_body->getAngularVelocity(),
		m_body->getLinearFactor(),
		m_body->getAngularFactor(),
		GetBounciness(),
		GetFriction(),
		IsTrigger(),
		IsKinematic()
	};

	Unconsider();

	m_body.reset();
	m_motion.reset();

	return result;
}

btVector3 VisionPhysics::Entities::PhysicalObject::CalculateInertia() const
{
	btVector3 result = { 0.f, 0.f, 0.f };

	if (m_mass != 0.0f)
		m_shape->calculateLocalInertia(m_mass, result);

	return result;
}

btRigidBody& VisionPhysics::Entities::PhysicalObject::GetBody()
{
	return *m_body;
}
