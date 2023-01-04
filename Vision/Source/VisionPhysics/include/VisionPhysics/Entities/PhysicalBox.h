#pragma once

#include "VisionPhysics/Entities/PhysicalObject.h"

namespace VisionPhysics::Entities
{
	/**
	* PhysicalObject with a box shape
	*/
	class PhysicalBox : public PhysicalObject
	{
	public:
		/**
		* PhysicalBox constructor (Internal transform management)
		* @param p_radius
		* @param p_height
		*/
		PhysicalBox(const VisionMaths::FVector3& p_size = {0.5f, 0.5f, 0.5f});

		/**
		* PhysicalBox constructor (External transform management)
		* @param p_transform
		* @param p_radius
		* @param p_height
		*/
		PhysicalBox(VisionMaths::FTransform& p_transform, const VisionMaths::FVector3& p_size = { 0.5f, 0.5f, 0.5f });

		/**
		* Defines the size of the physical box
		* @param p_size
		*/
		void SetSize(const VisionMaths::FVector3& p_size);

		/**
		* Returns the size of the physical box
		*/
		VisionMaths::FVector3 GetSize() const;

	private:
		void CreateCollisionShape(const VisionMaths::FVector3& p_size);
		void RecreateCollisionShape(const VisionMaths::FVector3& p_size);
		virtual void SetLocalScaling(const VisionMaths::FVector3& p_scaling) override;

	private:
		VisionMaths::FVector3 m_size;
	};
}