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
	float centerDist = glm::length(other.GetCenter() - mCenter);
	glm::vec3 direction = (other.GetCenter() - mCenter);
	float dist = centerDist - radiusDist;
	direction /= centerDist;

	return IntersectData(dist < 0, direction * dist);

}
