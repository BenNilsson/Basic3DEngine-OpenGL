#include "Scene.h"

void Scene::AddGameObject(GameObject* object)
{
	objects.push_back(object);
}

void Scene::RemoveGameObject(GameObject* object)
{
	objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
}