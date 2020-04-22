#pragma once

//#include "RigidBody.h"

#include <vector>

class RigidBody;

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

	void Simulate(float delta);

	void HandleCollisions();

	inline const RigidBody& GetObject(unsigned int index) const
	{
		return *mRigidbodies[index];
	}

	inline unsigned int GetNumObjects() const
	{
		return (unsigned int)mRigidbodies.size();
	}


private:
	static PhysicsEngine* instance;

	void Init()
	{
		mRigidbodies = std::vector<RigidBody*>();
	}

	std::vector<RigidBody*> mRigidbodies;
};