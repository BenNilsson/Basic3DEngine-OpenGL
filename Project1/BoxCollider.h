#pragma once

#include <glm/glm.hpp>

#include "GameComponent.h"
#include "IntersectData.h"

class BoxCollider : GameComponent
{
public:

	BoxCollider(const glm::vec3& minExtents, const glm::vec3& maxExtents) : mMinExtents(minExtents), mMaxExtents(maxExtents) {}

	IntersectData IntersectBoxCollider(const BoxCollider& other) const;

	inline const glm::vec3& GetMinExtents() const { return mMinExtents; }
	inline const glm::vec3& GetMaxExtents() const { return mMaxExtents; }

private:
	const glm::vec3 mMinExtents;
	const glm::vec3 mMaxExtents;
};
