#pragma once
#ifndef SPHERECOLLIDER_H
#define SPHERECOLLIDER_H

#include <glm/glm.hpp>

#include "GameComponent.h"
#include "IntersectData.h"

class SphereCollider : GameComponent
{
public:

	SphereCollider(const glm::vec3& center, float radius) : mCenter(center), mRadius(radius) {}

	IntersectData IntersectSphereCollider(const SphereCollider& other);

	inline const glm::vec3& GetCenter() const { return mCenter; }
	inline float GetRadius() const { return mRadius; }

private:
	const glm::vec3 mCenter;
	const float mRadius;
};

#endif
