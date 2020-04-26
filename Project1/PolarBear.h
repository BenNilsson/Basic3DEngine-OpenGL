#pragma once
#include "GameObject.h"
#include "RigidBody.h"

class PolarBear : GameObject
{
public:

	PolarBear(std::string modelPath, Transform transform, Shader* shader, Scene* scene);
	~PolarBear();


	void Render(glm::mat4 _model);
	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void HandleInput(GLFWwindow* window, float deltaTime);

	inline bool GetIsMoving() { return mIsMoving = mRigidbody.mVelocity.x != 0.0f || mRigidbody.mVelocity.z != 0.0f; }

	// Callbacks
	//virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	//virtual void scroll_callback(GLFWwindow* window, double offset, double yoffset);
	//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void Destroy();

	float mMovementSpeed = 4.0f;
	RigidBody mRigidbody;
private:

	bool mIsMoving;
	SphereCollider* mSphereCollider;
	float mRotationAmount;
	GameObject* gfx;
	std::vector<glm::vec3> mWaypoints;

	glm::vec3 mWayPointToMoveTo;
	bool mHasReachedWaypoint;
	int mWaypointIndex;

	glm::mat4 matrixTransformation;
};

