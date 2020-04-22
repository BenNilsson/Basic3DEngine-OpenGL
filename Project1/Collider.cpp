#include "Collider.h"

#include "BoxCollider.h"
#include "SphereCollider.h"

#include <iostream>

IntersectData Collider::Intersect(const Collider& other) const
{
	// Major place to handle any form of collision, box vs box, box vs sphere and so on
	if (mType == COLLIDER_SPHERE && other.GetType() == COLLIDER_SPHERE)
	{
		SphereCollider* self = (SphereCollider*)this;
		return self->IntersectSphereCollider((SphereCollider&)other);
	}
	

	// Print message for collider type not being supported
	std::cerr << "Error: Collision not implemented between type " << mType << " & " << other.GetType() << std::endl;

	return IntersectData(false, glm::vec3(0.0f));
}
