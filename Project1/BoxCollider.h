#pragma once

#include <glm/glm.hpp>

#include "Collider.h"

class BoxCollider : public Collider
{
public:

	BoxCollider(const glm::vec3& minExtents, const glm::vec3& maxExtents) : Collider(Collider::COLLIDER_BOX), mMinExtents(minExtents), mMaxExtents(maxExtents) {}

	IntersectData IntersectBoxCollider(const BoxCollider& other) const;

	virtual glm::vec3 GetCenter() const { return glm::vec3(0.0f); }

	inline const glm::vec3& GetMinExtents() const { return mMinExtents; }
	inline const glm::vec3& GetMaxExtents() const { return mMaxExtents; }

private:
	const glm::vec3 mMinExtents;
	const glm::vec3 mMaxExtents;
};
