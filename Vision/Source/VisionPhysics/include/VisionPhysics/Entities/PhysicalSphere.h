#pragma once

#include "VisionPhysics/Entities/PhysicalObject.h"

namespace VisionPhysics::Entities
{
	/**
	* PhysicalObject with a sphere shape
	*/
	class PhysicalSphere : public PhysicalObject
	{
	public:
		/**
		* PhysicalSphere constructor (Internal transform management)
		* @param p_radius
		* @param p_height
		*/
		PhysicalSphere(float p_radius = 1.f);

		/**
		* PhysicalSphere constructor (External transform management)
		* @param p_transform
		* @param p_radius
		* @param p_height
		*/
		PhysicalSphere(VisionMaths::FTransform& p_transform, float p_radius = 1.f);

		/**
		* Defines a new radius for the sphere
		* @param p_radius
		*/
		void SetRadius(float p_radius);

		/**
		* Returns the actual radius of the sphere
		*/
		float GetRadius() const;

	private:
		void CreateCollisionShape(float p_radius);
		void RecreateCollisionShape(float p_radius);
		virtual void SetLocalScaling(const VisionMaths::FVector3& p_scaling) override;

	private:
		float m_radius;
	};
}