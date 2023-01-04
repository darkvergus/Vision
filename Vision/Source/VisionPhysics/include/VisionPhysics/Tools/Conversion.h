#pragma once

#include <bullet/btBulletCollisionCommon.h>

#include <VisionMaths/FTransform.h>
#include <VisionMaths/FVector3.h>
#include <VisionMaths/FQuaternion.h>

namespace VisionPhysics::Tools
{
	/**
	* Conversion helper to convert bullet maths to VisionMaths
	*/
	class Conversion
	{
	public:
		Conversion() = delete;

		/**
		* Convert a FTransform to btTransform
		* @param p_transform
		*/
		static btTransform ToBtTransform(const VisionMaths::FTransform& p_transform);

		/**
		* Convert a FVector3 to btVector3
		* @param p_vector
		*/
		static btVector3 ToBtVector3(const VisionMaths::FVector3& p_vector);

		/**
		* Convert a FQuaternion to btQuaternion
		* @param p_quaternion
		*/
		static btQuaternion ToBtQuaternion(const VisionMaths::FQuaternion& p_quaternion);

		/**
		* Convert a btTransform to FTransform
		* @param p_transform
		*/
		static VisionMaths::FTransform ToOvTransform(const btTransform& p_transform);

		/**
		* Convert a btVector3 to FVector3
		* @param p_vector
		*/
		static VisionMaths::FVector3 ToOvVector3(const btVector3& p_vector);

		/**
		* Convert a btQuaternion to FQuaternion
		* @param p_quaternion
		*/
		static VisionMaths::FQuaternion ToOvQuaternion(const btQuaternion& p_quaternion);
	};
}