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

IntersectData SphereCollider::IntersectBoxCollider(const BoxCollider& other)
{
	// Get box's closest point to the sphere's center by clamping
	float x = glm::max(other.GetMinExtents().x, glm::min(mCenter.x, other.GetMaxExtents().x));
	float y = glm::max(other.GetMinExtents().y, glm::min(mCenter.y, other.GetMaxExtents().y));
	float z = glm::max(other.GetMinExtents().z, glm::min(mCenter.z, other.GetMaxExtents().z));

	// Check if the point intersect the sphere
	float dist = glm::sqrt((x - mCenter.x) * (x - mCenter.x) +
		(y - mCenter.y) * (y - mCenter.y) +
		(z - mCenter.z) * (z - mCenter.z));

	return IntersectData(dist < mRadius, dist);
}
