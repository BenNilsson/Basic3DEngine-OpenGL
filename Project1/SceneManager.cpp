#include "SceneManager.h"
#include "Scene.h"

// Initialise static variables
SceneManager* SceneManager::instance = 0;

void SceneManager::Update(float deltaTime)
{
	mCurrentScene->Update(deltaTime);
}

void SceneManager::FixedUpdate(float deltaTime)
{
	mCurrentScene->FixedUpdate(deltaTime);
}

void SceneManager::Render()
{
	mCurrentScene->Render();
}

void SceneManager::HandleInput(GLFWwindow* window, float deltaTime)
{
	mCurrentScene->HandleInputs(window, deltaTime);
}

void SceneManager::ChangeScene(int order)
{
	if (mCurrentScene != NULL)
		delete mCurrentScene;

	mCurrentScene = scenes[order];
}

void SceneManager::ChangeScene(std::string sceneName)
{

}

void SceneManager::AddScene(Scene* scene)
{
	scenes.push_back(scene);
}

void SceneManager::RemoveScene(int order)
{
	scenes.erase(scenes.begin() + order);
}

