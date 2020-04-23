#include "PhysicsEngine.h"
#include "SphereCollider.h"

#include "RigidBody.h"

#include <iostream>

PhysicsEngine* PhysicsEngine::instance = 0;

void PhysicsEngine::AddRigidbody(RigidBody* rigidbody)
{
	mRigidbodies.push_back(rigidbody);
}

void PhysicsEngine::ApplyForces(float deltaTime)
{
	for (unsigned int i = 0; i < mRigidbodies.size(); i++)
	{
		glm::vec3 force = ComputeForce(mRigidbodies[i]);
		glm::vec3 acceleration = glm::vec3(force.x / mRigidbodies[i]->mMass, force.y / mRigidbodies[i]->mMass, force.z / mRigidbodies[i]->mMass);

		mRigidbodies[i]->mVelocity += acceleration * deltaTime;
		mRigidbodies[i]->mPosition += mRigidbodies[i]->mVelocity * deltaTime;
	}
}

void PhysicsEngine::Simulate(float deltaTime)
{
	for (unsigned int i = 0; i < mRigidbodies.size(); i++)
	{
		mRigidbodies[i]->Integrate(deltaTime);
	}
}

void PhysicsEngine::HandleCollisions()
{
	for (unsigned int i = 0; i < mRigidbodies.size(); i++)
	{
		for (unsigned int j = i + 1; j < mRigidbodies.size(); j++)
		{
			if (mRigidbodies[i]->mCollider == nullptr) return;
			// Check whether stuff collides, then handle responses
			IntersectData intersectData = mRigidbodies[i]->GetCollider().Intersect(mRigidbodies[j]->GetCollider());

			if (intersectData.GetDoesIntersect())
			{
				glm::vec3 dir = glm::normalize(intersectData.GetDirection());
				glm::vec3 otherDir = glm::reflect(dir, glm::normalize(mRigidbodies[i]->GetVelocity()));

				mRigidbodies[i]->mVelocity = glm::reflect(otherDir, mRigidbodies[i]->mVelocity);
				mRigidbodies[i]->mPosition = mRigidbodies[i]->GetOldPos();

				// Set the position back to its previous position to avoid the collision response from triggering twice
				//mRigidbodies[j]->mVelocity = glm::reflect(dir, mRigidbodies[j]->mVelocity);
				//mRigidbodies[j]->mPosition = mRigidbodies[j]->GetOldPos();
				
				// TODO
				// Implementation of mass, if an object has a higher mass, apply less force to it and more to others?

				// TODO
				// Maybe send call a generic function to each rigidbody that returns the rigidbodies' as a "collision response".
				// The rb could then send the same function to its parent gameObject
			}
		}
	}
}

glm::vec3 PhysicsEngine::ComputeForce(RigidBody* rb)
{
	if(rb->mEnableGravity)
		return glm::vec3(0.0f, rb->mMass * gravity, 0.0f);
	else return glm::vec3(0.0f, 0.0f, 0.0f);
	
}
