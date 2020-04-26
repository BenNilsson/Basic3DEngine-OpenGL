#include "GameObject.h"
#include "Camera.h"

#include "Scene.h"

#include <algorithm>

GameObject::GameObject(std::string modelPath, Transform transform, Shader* shader, Scene* scene) : mShader(shader), mTransform(transform)
{
	if (modelPath != "")
		mModel = new Model((char*)modelPath.c_str());
	else mModel = nullptr;

	

	childObjects = std::vector<GameObject*>();

	mScene = scene;

	// Render Model
	mShader->use();

	// Set Shader's Colors
	mShader->setVec3("viewPos", Camera::GetInstance()->Position);
	mShader->setFloat("material.shininess", 32.0f);
}

GameObject::~GameObject()
{
	delete mModel;
	mModel = nullptr;

	delete mShader;
	mShader = nullptr;

	for (unsigned int i = 0; i < childObjects.size(); i++)
	{
		delete childObjects[i];
		childObjects[i] = nullptr;
	}

	for (unsigned int i = 0; i < gameComponents.size(); i++)
	{
		delete gameComponents[i];
		gameComponents[i] = nullptr;
	}

	delete mScene;
	mScene = nullptr;
}

void GameObject::Update(float deltaTime)
{
	// Update child objects
	for (unsigned int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->Update(deltaTime);
	}

	// Update components
	for (unsigned int i = 0; i < gameComponents.size(); i++)
	{
		gameComponents[i]->Update(deltaTime);
	}
}

void GameObject::FixedUpdate(float deltaTime)
{
	// Update child objects
	for (unsigned int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->FixedUpdate(deltaTime);
	}
}

void GameObject::Render(glm::mat4 _model)
{
	if (mShader == nullptr) return;

	// Render Model
	mShader->use();

	// View/Projection Transforms
	glm::mat4 projection = glm::perspective(glm::radians(Camera::GetInstance()->Zoom), (float)Camera::GetInstance()->SCREEN_WIDTH / (float)Camera::GetInstance()->SCREEN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = Camera::GetInstance()->GetViewMatrix();
	mShader->setMat4("projection", projection);
	mShader->setMat4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	// Translate
	model = glm::translate(model, mTransform.position);
	// Rotation - x, y, z
	model = glm::rotate(model, glm::radians(mTransform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(mTransform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(mTransform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	// Scale
	model = glm::scale(model, mTransform.scale);

	model = _model * model;
	mShader->setMat4("model", model);

	glm::mat4 inverted = glm::inverse(model);
	mTransform.forward = normalize(glm::vec3(inverted[2]) * glm::vec3(-1, 1, 1));

	// Render Model
	if(mModel != nullptr)
		mModel->Render(*mShader);
	
	// Render Child Objects
	for (unsigned int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->Render(model);
	}

	// Render components
	for (unsigned int i = 0; i < gameComponents.size(); i++)
	{
		gameComponents[i]->Render();
	}
}

void GameObject::HandleInput(GLFWwindow* window, float deltaTime)
{
	// Child input
	for (unsigned int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->HandleInput(window, deltaTime);
	}

	// Component input
	for (unsigned int i = 0; i < gameComponents.size(); i++)
	{
		gameComponents[i]->HandleInput(window, deltaTime);
	}
}

void GameObject::AddChild(GameObject* child)
{
	childObjects.push_back(child);
}

void GameObject::RemoveChild(GameObject* child)
{
	childObjects.erase(std::remove(childObjects.begin(), childObjects.end(), child), childObjects.end());
}

void GameObject::AddComponent(GameComponent* component)
{
	gameComponents.push_back(component);
}

GameComponent* GameObject::GetComponent(GameComponent* component)
{
	GameComponent* result;

	auto it = std::find(gameComponents.begin(), gameComponents.end(), component);
	
	if (it != gameComponents.end())
		result = component;
	else result = nullptr;

	return result;
}

GameComponent* GameObject::GetComponentInChild(GameComponent* component, int childIndex)
{
	if (childObjects[childIndex] == nullptr) return nullptr;

	GameComponent* result;

	auto it = std::find(childObjects[childIndex]->gameComponents.begin(), childObjects[childIndex]->gameComponents.end(), component);

	if (it != childObjects[childIndex]->gameComponents.end())
		result = component;
	else result = nullptr;

	return result;
}

void GameObject::Destroy()
{
	mScene->RemoveGameObject(this);

	for (unsigned int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->Destroy();
	}
}

void GameObject::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Child callback
	for (unsigned int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->mouse_callback(window, xpos, ypos);
	}
}

void GameObject::scroll_callback(GLFWwindow* window, double offset, double yoffset)
{
	// Child callback
	for (unsigned int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->scroll_callback(window, offset, yoffset);
	}
}

void GameObject::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Child callback
	for (unsigned int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->key_callback(window, key, scancode, action, mods);
	}
}
