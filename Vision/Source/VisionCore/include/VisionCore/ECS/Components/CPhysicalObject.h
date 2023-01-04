#pragma once

#include <VisionPhysics/Entities/PhysicalObject.h>

#include "VisionCore/ECS/Components/AComponent.h"

namespace VisionCore::ECS { class Actor; }

namespace VisionCore::ECS::Components
{
	/**
	* Base class for any physical object
	*/
	class CPhysicalObject : public AComponent
	{
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		CPhysicalObject(ECS::Actor& p_owner);

		/**
		* Add a force to the rigidbody
		* @param p_force
		*/
		void AddForce(const VisionMaths::FVector3& p_force);

		/**
		* Add an impulse to the rigidbody
		* @param p_impulse
		*/
		void AddImpulse(const VisionMaths::FVector3& p_impulse);

		/**
		* Clear forces
		*/
		void ClearForces();

		/**
		* Returns the mass of the physical object
		*/
		float GetMass() const;

		/**
		* Returns the collision detection mode of the physical object
		*/
		VisionPhysics::Entities::PhysicalObject::ECollisionDetectionMode GetCollisionDetectionMode() const;

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
		VisionPhysics::Entities::PhysicalObject::EActivationState GetActivationState() const;

		/**
		* Defines a new mass for the physical object
		* @param p_mass
		*/
		void SetMass(float p_mass);

		/**
		* Defines a new collision detection mode for the physical object
		* @param p_mode
		*/
		void SetCollisionDetectionMode(VisionPhysics::Entities::PhysicalObject::ECollisionDetectionMode p_mode);

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
		void SetActivationState(VisionPhysics::Entities::PhysicalObject::EActivationState p_activationState);

		/**
		* Serialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		/**
		* Deserialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		/**
		* Defines how the component should be drawn in the inspector
		* @param p_root
		*/
		virtual void OnInspector(VisionUI::Internal::WidgetContainer& p_root) override;

	protected:
		template <typename T> T& GetPhysicalObjectAs() const { return *static_cast<T*>(m_physicalObject.get()); }
		void Init();
		void BindListener();

	private:
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	public:
		VisionTools::Eventing::Event<CPhysicalObject&> CollisionEnterEvent;
		VisionTools::Eventing::Event<CPhysicalObject&> CollisionStayEvent;
		VisionTools::Eventing::Event<CPhysicalObject&> CollisionExitEvent;

		VisionTools::Eventing::Event<CPhysicalObject&> TriggerEnterEvent;
		VisionTools::Eventing::Event<CPhysicalObject&> TriggerStayEvent;
		VisionTools::Eventing::Event<CPhysicalObject&> TriggerExitEvent;

	protected:
		std::unique_ptr<VisionPhysics::Entities::PhysicalObject> m_physicalObject;
	};
}