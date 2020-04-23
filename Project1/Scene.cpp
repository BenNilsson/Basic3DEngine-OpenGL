#include "Scene.h"

Scene::Scene(std::string name, GLFWwindow* window)
{
	mName = name; 
	mWindow = window;
}

Scene::~Scene()
{
}

void Scene::Render()
{
	// Render game objects
	for (unsigned int i = 0; i < objects.size(); i++)
		objects[i]->Render(glm::mat4(1.0f));
}

void Scene::Update(float deltaTime)
{
	// Update game objects
	for (unsigned int i = 0; i < objects.size(); i++)
		objects[i]->Update(deltaTime);
}

void Scene::FixedUpdate(float deltaTime)
{
	// Update game objects
	for (unsigned int i = 0; i < objects.size(); i++)
		objects[i]->FixedUpdate(deltaTime);
}

void Scene::AddGameObject(GameObject* object)
{
	objects.push_back(object);
}

void Scene::RemoveGameObject(GameObject* object)
{
	objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
}