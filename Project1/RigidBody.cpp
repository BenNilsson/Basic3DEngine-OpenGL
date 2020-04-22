#include "RigidBody.h"

#include <iostream>

RigidBody::~RigidBody()
{
	delete mCollider;
	mCollider = nullptr;
}

RigidBody::RigidBody(const RigidBody& other) :
	mPosition(other.mPosition),
	mOldPosition(other.mOldPosition),
	mVelocity(other.mVelocity),
	mCollider(other.mCollider) { }

void RigidBody::Update(float deltaTime)
{
	std::cout << mCollider->GetCenter().x << " | " << mCollider->GetCenter().y << " | " << mCollider->GetCenter().z << 
	" | " << mPosition.x << " | " << mPosition.y << " | " << mPosition.z <<
	std::endl;
	
}


void RigidBody::Integrate(float deltaTime)
{
	// Move rb position based on velocity
	mPosition += mVelocity * deltaTime;
}
