#pragma once
#include "GameObject.h"
#include "RigidBody.h"

class Penguin : GameObject
{
public:

	Penguin(std::string modelPath, Transform transform, Shader* shader, Scene* scene);
	~Penguin();

	void Update(float deltaTime);
	void HandleInput(GLFWwindow* window, float deltaTime);

	// Callbacks
	//virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	//virtual void scroll_callback(GLFWwindow* window, double offset, double yoffset);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void Destroy();

	float mMovementSpeed = 4.0f;
	RigidBody mRigidbody;
private:

	SphereCollider* mSphereCollider;

	GameObject* penguinGFX;
	GameObject* ak47;

	bool mFreeze;

};

