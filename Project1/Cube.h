#pragma once

#include "GameObject.h"
#include "RigidBody.h"

class Cube : GameObject
{
public:
	Cube(std::string modelPath, Transform transform, Shader* shader, Scene* scene);
	~Cube();

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	//void HandleInput(GLFWwindow* window, float deltaTime);

	// Callbacks
	//virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	//virtual void scroll_callback(GLFWwindow* window, double offset, double yoffset);
	//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void Destroy()
	{
		if (this != nullptr)
			GameObject::Destroy();
	}

	RigidBody mRigidbody;

private:
	//TODO: Change to box collider
	SphereCollider* mSphereCollider;
};