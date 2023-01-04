#pragma once

#include <any>
#include <memory>

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

#include <VisionMaths/FTransform.h>

#include <VisionTools/Eventing/Event.h>


#include "VisionPhysics/Settings/BodySettings.h"

namespace VisionPhysics::Core { class PhysicsEngine; }

namespace VisionPhysics::Entities
{
	/**
	* Base class for any entity that is physical
	*/
	class PhysicalObject
	{
	public:
		friend class VisionPhysics::Core::PhysicsEngine;

		/**
		* Defines some collision detection modes that a physical object can have
		*/
		enum class ECollisionDetectionMode
		{
			DISCRETE,
			CONTINUOUS
		};

		/**
		* Defines some activation states that a physical object can have
		*/
		enum class EActivationState
		{
			ACTIVE				= 1,
			SLEEPING			= 2,
			LOOKING_FOR_SLEEP	= 3,
			ALWAYS_ACTIVE		= 4,
			ALWAYS_SLEEPING		= 5
		};

		/**
		* PhysicalObject constructor (Internal transform management)
		*/
		PhysicalObject();

		/**
		* PhysicalObject constructor (External transform management)
		* @param p_shapeType
		* @param p_transform
		*/
		PhysicalObject(VisionMaths::FTransform& p_transform);

		/**
		* PhysicalObject destructor (Free memory if the transform is internally managed)
		*/
		~PhysicalObject();

		/**
		* Add a force to the physical object
		* @param p_force
		*/
		void AddForce(const VisionMaths::FVector3& p_force);

		/**
		* Add an impulse to the physical object
		* @param p_impulse
		*/
		void AddImpulse(const VisionMaths::FVector3& p_impulse);

		/**
		* Clear forces
		*/
		void ClearForces();

		/**
		* Add a flag to the physical object
		* @param p_flag
		*/
		void AddFlag(btCollisionObject::CollisionFlags p_flag);

		/**
		* Add a flag from the physical object
		* @param p_flag
		*/
		void RemoveFlag(btCollisionObject::CollisionFlags p_flag);

		/**
		* Returns the mass of the physical object
		*/
		float GetMass() const;

		/**
		* Returns the collision detection mode of the physical object
		*/
		const ECollisionDetectionMode& GetCollisionDetectionMode() const;

		/**
		* Returns the bounciness of the physical object
		*/
		float GetBounciness() const;

		/**
		* Returns the friction of the physical object
		*/
		float GetFriction()	const;

		/**
		* Returns the linear velocity of the physical object
		*/
		VisionMaths::FVector3 GetLinearVelocity() const;

		/**
		* Returns the angular of the physical object
		*/
		VisionMaths::FVector3 GetAngularVelocity() const;

		/**
		* Returns the linear factor of the physical object
		*/
		VisionMaths::FVector3 GetLinearFactor()	const;

		/**
		* Returns the angular factor of the physical object
		*/
		VisionMaths::FVector3 GetAngularFactor() const;

		/**
		* Returns true if the physical object is a trigger
		*/
		bool IsTrigger() const;

		/**
		* Returns true if the physical object is kinematic
		*/
		bool IsKinematic() const;

		/**
		* Returns the current activation state
		*/
		EActivationState GetActivationState() const;

		/**
		* Returns a reference to the transform of the physical object
		*/
		VisionMaths::FTransform& GetTransform();

		/**
		* Defines a new mass for the physical object
		* @param p_mass
		*/
		void SetMass(float p_mass);

		/**
		* Defines a new collision detection mode for the physical object
		* @param p_mode
		*/
		void SetCollisionDetectionMode(ECollisionDetectionMode p_mode);

		/**
		* Defines a new bounciness for the physical object
		* @param p_bounciness
		*/
		void SetBounciness(float p_bounciness);

		/**
		* Defines a new friction for the physical object
		* @param p_mass
		*/
		void SetFriction(float p_friction);

		/**
		* Defines a new linear velocity for the physical object
		* @param p_linearVelocity
		*/
		void SetLinearVelocity(const VisionMaths::FVector3& p_linearVelocity);

		/**
		* Defines a new angular velocity for the physical object
		* @param p_angularVelocity
		*/
		void SetAngularVelocity(const VisionMaths::FVector3& p_angularVelocity);

		/**
		* Defines a new linear factor for the physical object
		* @param p_linearFactor
		*/
		void SetLinearFactor(const VisionMaths::FVector3& p_linearFactor);

		/**
		* Defines a new angular factor for the physical object
		* @param p_angularFactor
		*/
		void SetAngularFactor(const VisionMaths::FVector3& p_angularFactor);

		/**
		* Defines if the physical object should be a trigger or not
		* @param p_trigger
		*/
		void SetTrigger(bool p_trigger);

		/**
		* Defines if the physical object should be kinematic or not
		* @param p_kinematic
		*/
		void SetKinematic(bool p_kinematic);

		/**
		* Defines the new activation state for the physical object
		* @param p_activationState
		*/
		void SetActivationState(EActivationState p_activationState);

		/**
		* Defines if the physical object should be enabled or not
		* @param p_enabled
		*/
		void SetEnabled(bool p_enabled);

		/**
		* Returns true if the physical object is enabled
		*/
		bool IsEnabled() const;

		/**
		* Returns the user data associated to this physical object instance
		*/
		template <typename T> T		GetUserData() const		{ return std::any_cast<T>(m_userData); }

		/**
		* Defines the user data associated to this physical object instance
		* @param p_data
		*/
		template <typename T> void	SetUserData(T p_data)	{ m_userData = p_data; }

	protected:
		void Init();
		void RecreateBody();
		void ApplyInertia();
		virtual void SetLocalScaling(const VisionMaths::FVector3& p_scaling) = 0;
		void Consider();
		void Unconsider();

	private:
		/* Internal */
		void								CreateBody(const Settings::BodySettings& p_bodySettings);
		Settings::BodySettings				DestroyBody();
		btVector3							CalculateInertia() const;

		/* Needed by the physics engine */
		btRigidBody&			GetBody();
		void					UpdateBtTransform();
		void					UpdateFTransform();

	public:
		VisionTools::Eventing::Event<PhysicalObject&>			CollisionStartEvent;
		VisionTools::Eventing::Event<PhysicalObject&>			CollisionStayEvent;
		VisionTools::Eventing::Event<PhysicalObject&>			CollisionStopEvent;

		VisionTools::Eventing::Event<PhysicalObject&>			TriggerStartEvent;
		VisionTools::Eventing::Event<PhysicalObject&>			TriggerStayEvent;
		VisionTools::Eventing::Event<PhysicalObject&>			TriggerStopEvent;

	private:
		/* Transform stuff */
		VisionMaths::FTransform* const		m_transform;
		const bool						m_internalTransform;

		/* Settings */
		float					m_mass = 1.f;
		bool					m_kinematic = false;
		bool					m_trigger = false;
		bool					m_enabled = true;
		bool					m_considered = false;
		ECollisionDetectionMode m_collisionMode = ECollisionDetectionMode::DISCRETE;

		/* Other */
		std::any m_userData;
		VisionMaths::FVector3 m_previousScale = { 0.0f, 0.0f, 0.0f };
		static VisionTools::Eventing::Event<PhysicalObject&>	CreatedEvent;
		static VisionTools::Eventing::Event<PhysicalObject&>	DestroyedEvent;
		static VisionTools::Eventing::Event<btRigidBody&>		ConsiderEvent;
		static VisionTools::Eventing::Event<btRigidBody&>		UnconsiderEvent;

		/* Bullet relatives */
		std::unique_ptr<btMotionState>		m_motion;
		std::unique_ptr<btRigidBody>		m_body;

	protected:
		std::unique_ptr<btCollisionShape>	m_shape;
	};
}