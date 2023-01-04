#include <algorithm>

#include "VisionPhysics/Core/PhysicsEngine.h"
#include "VisionPhysics/Tools/Conversion.h"
#include "VisionPhysics/Entities/PhysicalObject.h"

#include <VisionDebug/Logger.h>

using namespace VisionPhysics::Tools;
using namespace VisionPhysics::Entities;

std::map< std::pair<PhysicalObject*, PhysicalObject*>, bool> VisionPhysics::Core::PhysicsEngine::m_collisionEvents;

VisionPhysics::Core::PhysicsEngine::PhysicsEngine(const Settings::PhysicsSettings & p_settings)
{
	m_collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
	m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfig.get());
	m_broadphase = std::make_unique<btDbvtBroadphase>();
	m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
	m_world = std::make_unique<btDiscreteDynamicsWorld>(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfig.get());

	m_world->setGravity(Conversion::ToBtVector3(p_settings.gravity));

	ListenToPhysicalObjects();
	SetCollisionCallback();
}

void VisionPhysics::Core::PhysicsEngine::PreUpdate()
{
	std::for_each(m_physicalObjects.begin(), m_physicalObjects.end(), std::mem_fn(&PhysicalObject::UpdateBtTransform));

	ResetCollisionEvents();
}

void VisionPhysics::Core::PhysicsEngine::PostUpdate()
{
	std::for_each(m_physicalObjects.begin(), m_physicalObjects.end(), std::mem_fn(&PhysicalObject::UpdateFTransform));

	CheckCollisionStopEvents();
}

bool VisionPhysics::Core::PhysicsEngine::Update(float p_deltaTime)
{
	PreUpdate();

	if (m_world->stepSimulation(p_deltaTime, 10))
	{
		PostUpdate();
		return true;
	}

	return false;
}

std::optional<RaycastHit> VisionPhysics::Core::PhysicsEngine::Raycast(VisionMaths::FVector3 p_origin, VisionMaths::FVector3 p_direction, float p_distance)
{
	if (p_direction == VisionMaths::FVector3::Zero)
		return {};

	btVector3 origin = Tools::Conversion::ToBtVector3(p_origin);
	btVector3 target = Tools::Conversion::ToBtVector3(p_origin + p_direction * p_distance);

	RaycastHit resultHit;

	// Try to get First Hit
	btCollisionWorld::ClosestRayResultCallback ClosestRayCallback(origin, target);
	m_world->rayTest(origin, target, ClosestRayCallback);

	if (ClosestRayCallback.hasHit())
	{
		// Get First Hit
		resultHit.FirstResultObject = reinterpret_cast<VisionPhysics::Entities::PhysicalObject*>(ClosestRayCallback.m_collisionObject->getUserPointer());

		// Try to get all Hit
		btCollisionWorld::AllHitsRayResultCallback rayCallback(origin, target);
		m_world->rayTest(origin, target, rayCallback);

		// Get all Hit
		for (int i = 0; i < rayCallback.m_collisionObjects.size(); i++)
			resultHit.ResultObjects.push_back(reinterpret_cast<VisionPhysics::Entities::PhysicalObject*>(rayCallback.m_collisionObjects[i]->getUserPointer()));

		return resultHit;
	}
	else
		return {};
}

void VisionPhysics::Core::PhysicsEngine::SetGravity(const VisionMaths::FVector3 & p_gravity)
{
	m_world->setGravity(Conversion::ToBtVector3(p_gravity));
}

VisionMaths::FVector3 VisionPhysics::Core::PhysicsEngine::GetGravity() const
{
	return Conversion::ToOvVector3(m_world->getGravity());
}

void VisionPhysics::Core::PhysicsEngine::ListenToPhysicalObjects()
{
	PhysicalObject::CreatedEvent += std::bind(static_cast<void(PhysicsEngine::*)(PhysicalObject&)>(&PhysicsEngine::Consider), this, std::placeholders::_1);
	PhysicalObject::DestroyedEvent += std::bind(static_cast<void(PhysicsEngine::*)(PhysicalObject&)>(&PhysicsEngine::Unconsider), this, std::placeholders::_1);

	PhysicalObject::ConsiderEvent += std::bind(static_cast<void(PhysicsEngine::*)(btRigidBody&)>(&PhysicsEngine::Consider), this, std::placeholders::_1);
	PhysicalObject::UnconsiderEvent += std::bind(static_cast<void(PhysicsEngine::*)(btRigidBody&)>(&PhysicsEngine::Unconsider), this, std::placeholders::_1);
}

void VisionPhysics::Core::PhysicsEngine::Consider(PhysicalObject& p_toConsider)
{
	m_physicalObjects.push_back(std::ref(p_toConsider));
}

void VisionPhysics::Core::PhysicsEngine::Unconsider(PhysicalObject& p_toUnconsider)
{
	{
		auto found = std::find_if(m_physicalObjects.begin(), m_physicalObjects.end(), [&p_toUnconsider](std::reference_wrapper<PhysicalObject> element)
		{
			return std::addressof(p_toUnconsider) == std::addressof(element.get());
		});

		if (found != m_physicalObjects.end())
			m_physicalObjects.erase(found);
	}

	{
		decltype(m_collisionEvents)::iterator iter = m_collisionEvents.begin();
		decltype(m_collisionEvents)::iterator endIter = m_collisionEvents.end();

		for (; iter != endIter; )
		{
			if (iter->first.first == std::addressof(p_toUnconsider) || iter->first.second == std::addressof(p_toUnconsider))
			{
				m_collisionEvents.erase(iter++);
			}
			else {
				++iter;
			}
		}
	}
}

void VisionPhysics::Core::PhysicsEngine::Consider(btRigidBody& p_toConsider)
{
	m_world->addRigidBody(&p_toConsider);
}

void VisionPhysics::Core::PhysicsEngine::Unconsider(btRigidBody& p_toUnconsider)
{
	m_world->removeRigidBody(&p_toUnconsider);
}

void VisionPhysics::Core::PhysicsEngine::ResetCollisionEvents()
{
	for (auto& element : m_collisionEvents)
		element.second = false;
}

void VisionPhysics::Core::PhysicsEngine::CheckCollisionStopEvents()
{
	for (auto it = m_collisionEvents.begin(); it != m_collisionEvents.end();)
	{
		auto objects = it->first;
		if (!it->second)
		{
			if (!objects.first->IsTrigger() && !objects.second->IsTrigger())
			{
				objects.first->CollisionStopEvent.Invoke(*objects.second);
				objects.second->CollisionStopEvent.Invoke(*objects.first);
			}
			else
			{
				if (objects.first->IsTrigger())
					objects.first->TriggerStopEvent.Invoke(*objects.second);
				else
					objects.second->TriggerStopEvent.Invoke(*objects.first);
			}

			it = m_collisionEvents.erase(it);
		}
		else
			++it;
	}
}

bool VisionPhysics::Core::PhysicsEngine::CollisionCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2)
{
	auto object1 = reinterpret_cast<PhysicalObject*>(obj1->getCollisionObject()->getUserPointer());
	auto object2 = reinterpret_cast<PhysicalObject*>(obj2->getCollisionObject()->getUserPointer());

	if (object1 && object2)
	{
		/* If the objects are not all trigger, enter */
		if (!object1->IsTrigger() || !object2->IsTrigger())
		{
			if (m_collisionEvents.find({ object1 , object2 }) == m_collisionEvents.end())
			{
				/* If object is trigger, invoke Trigger event,
				 * else : is the other object trigger ? yes -> do nothing, no -> invoke Collision event
				 */

				 // Object 1 (Start event)
				if (object1->IsTrigger())
					object1->TriggerStartEvent.Invoke(*object2);
				else
				{
					if (!object2->IsTrigger())
						object1->CollisionStartEvent.Invoke(*object2);
				}
				// Object 2 (Start event)
				if (object2->IsTrigger())
					object2->TriggerStartEvent.Invoke(*object1);
				else
				{
					if (!object1->IsTrigger())
						object2->CollisionStartEvent.Invoke(*object1);
				}

				// Object 1 (Stay event)
				if (object1->IsTrigger())
					object1->TriggerStayEvent.Invoke(*object2);
				else
				{
					if (!object2->IsTrigger())
						object1->CollisionStayEvent.Invoke(*object2);
				}
				// Object 2 (Stay event)
				if (object2->IsTrigger())
					object2->TriggerStayEvent.Invoke(*object1);
				else
				{
					if (!object1->IsTrigger())
						object2->CollisionStayEvent.Invoke(*object1);
				}

				m_collisionEvents[{ object1, object2 }] = true;
			}
			else
			{
				if (!m_collisionEvents[{ object1, object2 }])
				{
					// Object 1 (Stay event)
					if (object1->IsTrigger())
						object1->TriggerStayEvent.Invoke(*object2);
					else
					{
						if (!object2->IsTrigger())
							object1->CollisionStayEvent.Invoke(*object2);
					}
					// Object 2 (Stay event)
					if (object2->IsTrigger())
						object2->TriggerStayEvent.Invoke(*object1);
					else
					{
						if (!object1->IsTrigger())
							object2->CollisionStayEvent.Invoke(*object1);
					}

					m_collisionEvents[{ object1, object2 }] = true;
				}
			}
		}
	}

	return false;
}

void VisionPhysics::Core::PhysicsEngine::SetCollisionCallback()
{
	gContactAddedCallback = &PhysicsEngine::CollisionCallback;
}
