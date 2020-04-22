#pragma once
#ifndef SPHERECOLLIDER_H
#define SPHERECOLLIDER_H

#include <glm/glm.hpp>

#include "Collider.h"

class SphereCollider : public Collider
{
public:

	SphereCollider(const glm::vec3& center, float radius) : Collider(Collider::COLLIDER_SPHERE), mCenter(center), mRadius(radius) {}

	void Transform(const glm::vec3& translation);

	IntersectData IntersectSphereCollider(const SphereCollider& other);

	void Update(float deltaTime) {}
	void Render() {}
	void HandleInput(GLFWwindow* window, float deltaTime) {}

	glm::vec3 GetCenter() const { return mCenter; }

	inline float GetRadius() const { return mRadius; }

	glm::vec3 mCenter;
private:
	float mRadius;
};

#endif
