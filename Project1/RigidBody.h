#pragma once

#include "GameComponent.h"
#include <glm/glm.hpp>

class RigidBody : GameComponent
{
public:
	RigidBody(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 velocity) : mPosition(position), mRotation(rotation), mVelocity(velocity) {}

	void Update(float deltaTime);

	inline const glm::vec3 GetPosition() const { return mPosition; }
	inline const glm::vec3 GetRotation() const { return mRotation; }
	inline const glm::vec3 GetVelocity() const { return mVelocity; }

	glm::vec3 mPosition;
	glm::vec3 mRotation;
	glm::vec3 mVelocity;
private:
};