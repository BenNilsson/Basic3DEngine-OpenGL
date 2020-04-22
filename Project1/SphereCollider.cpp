#include "SphereCollider.h"


#include <glm/glm.hpp>
#include <iostream>

void SphereCollider::Transform(const glm::vec3& translation)
{
	mCenter += translation;
}

IntersectData SphereCollider::IntersectSphereCollider(const SphereCollider& other)
{
	// Determine whether the two spheres are intersecting
	float radiusDist = mRadius + other.mRadius;
	glm::vec3 dir = (other.GetCenter() - mCenter);

	float centerDist = glm::length(dir);
	dir /= centerDist;

	float dist = centerDist - radiusDist;

	return IntersectData(dist < 0, dir * dist);
}
