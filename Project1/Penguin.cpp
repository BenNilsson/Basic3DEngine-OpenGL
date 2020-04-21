#include "Penguin.h"
#include "Camera.h"

Penguin::Penguin(std::string modelPath, Transform transform, Shader* shader, Scene* scene) : GameObject(modelPath, transform, shader, scene)
{
	mFreeze = false;

	penguinGFX = new GameObject((char*)"Objects/Penguinv2/Penguin.obj", Transform(glm::vec3(0.3f, 0.8f, 0.5f), glm::vec3(0.50f), glm::vec3(0.0f, -90.0f, 0.0f)), mShader, scene);
	AddChild(penguinGFX);

	ak47 = new GameObject((char*)"Objects/ak47/ak_47.obj", Transform(glm::vec3(0.3f, 0.8f, 0.5f), glm::vec3(0.50f), glm::vec3(0.0f)), mShader, scene);
	penguinGFX->AddChild(ak47);

	startPos = mTransform.position;
}

Penguin::~Penguin()
{
}

void Penguin::Update(float deltaTime)
{
	if (!mFreeze)
	{
		GameObject::Update(deltaTime);

		// Move forward
		mTransform.position += mTransform.forward * deltaTime * mMovementSpeed;
		mTransform.rotation.y += 0.5f;

		if (mTransform.rotation.y >= 360)
			mTransform.rotation.y = 0;
	}
		
}

void Penguin::HandleInput(GLFWwindow* window, float deltaTime)
{
	
}

void Penguin::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		mFreeze = !mFreeze;
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		Destroy();
	}
}

void Penguin::Destroy()
{
	if(this != nullptr)
		GameObject::Destroy();
}
