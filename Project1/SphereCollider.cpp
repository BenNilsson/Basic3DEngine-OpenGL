#include "SphereCollider.h"

IntersectData SphereCollider::IntersectSphereCollider(const SphereCollider& other)
{
	// Determine whether the two spheres are intersecting
	float radiusDist = mRadius + other.mRadius;
	float centerDist = (other.GetCenter() - mCenter).length();

	if (centerDist < radiusDist)
		return IntersectData(true, centerDist - radiusDist);
	else
		return IntersectData(false, centerDist - radiusDist);
}
