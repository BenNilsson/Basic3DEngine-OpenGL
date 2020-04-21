#include "RigidBody.h"

void RigidBody::Update(float deltaTime)
{
	// Move rb position based on velocity
	mPosition += mVelocity * deltaTime;
}
