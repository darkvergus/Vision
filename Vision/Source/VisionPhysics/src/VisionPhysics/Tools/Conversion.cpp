#include "VisionPhysics/Tools/Conversion.h"

btTransform VisionPhysics::Tools::Conversion::ToBtTransform(const VisionMaths::FTransform& p_transform)
{
	return btTransform(ToBtQuaternion(p_transform.GetWorldRotation()), ToBtVector3(p_transform.GetWorldPosition()));
}

btVector3 VisionPhysics::Tools::Conversion::ToBtVector3(const VisionMaths::FVector3& p_vector)
{
	return btVector3(p_vector.x, p_vector.y, p_vector.z);
}

btQuaternion VisionPhysics::Tools::Conversion::ToBtQuaternion(const VisionMaths::FQuaternion& p_quaternion)
{
	return btQuaternion(p_quaternion.x, p_quaternion.y, p_quaternion.z, p_quaternion.w);
}

VisionMaths::FTransform VisionPhysics::Tools::Conversion::ToOvTransform(const btTransform& p_transform)
{
	VisionMaths::FQuaternion q = ToOvQuaternion(p_transform.getRotation());
	VisionMaths::FVector3  v = ToOvVector3(p_transform.getOrigin());
	return VisionMaths::FTransform(v, q);
}

VisionMaths::FVector3 VisionPhysics::Tools::Conversion::ToOvVector3(const btVector3& p_vector)
{
	return VisionMaths::FVector3(p_vector.getX(), p_vector.getY(), p_vector.getZ());
}

VisionMaths::FQuaternion VisionPhysics::Tools::Conversion::ToOvQuaternion(const btQuaternion& p_quaternion)
{
	return VisionMaths::FQuaternion(p_quaternion.getX(), p_quaternion.getY(), p_quaternion.getZ(), p_quaternion.getW());
}
