#pragma once

//#include "RigidBody.h"

#include <vector>
#include <glm/glm.hpp>

class RigidBody;

// Simulate earth's gravity
const float gravity = -9.81;

class PhysicsEngine
{
public:

	static PhysicsEngine* GetInstance()
	{
		if (instance == 0)
		{
			instance = new PhysicsEngine();
			instance->Init();
		}

		return instance;
	}


	void AddRigidbody(RigidBody* object);

	void ApplyForces(float deltaTime);
	void Simulate(float deltaTime);
	void HandleCollisions();

	inline const RigidBody& GetObject(unsigned int index) const
	{
		return *mRigidbodies[index];
	}

	inline unsigned int GetNumObjects() const
	{
		return (unsigned int)mRigidbodies.size();
	}

	glm::vec3 ComputeForce(RigidBody* rb);


private:
	static PhysicsEngine* instance;

	void Init()
	{
		mRigidbodies = std::vector<RigidBody*>();
	}

	
	std::vector<RigidBody*> mRigidbodies;
};