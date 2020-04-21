#include "BoxCollider.h"

IntersectData BoxCollider::IntersectBoxCollider(const BoxCollider& other) const
{
	// Determine whether the two box colliders are intersecting

	// Get distance between the 2 boxes on each axis
	glm::vec3 dists1 = other.GetMinExtents() - mMaxExtents;
	glm::vec3 dists2 = mMinExtents - other.GetMaxExtents();

	glm::vec3 distances = glm::max(dists1, dists2);

	float maxDist = distances.x;
	if (distances.y > maxDist) maxDist = distances.y;
	if (distances.z > maxDist) maxDist = distances.z;

	return IntersectData(maxDist < 0, maxDist);
}
