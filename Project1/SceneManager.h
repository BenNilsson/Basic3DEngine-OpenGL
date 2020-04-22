#pragma once

#include "SceneLevel1.h"

#include <string>
#include <vector>
#include <iostream>

class Scene;

class SceneManager
{
public:
	
	static SceneManager* GetInstance()
	{
		if (instance == 0)
		{
			instance = new SceneManager();
			// Initialise values if needed
			instance->SetupScenes();
		}

		return instance;
	}

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);
	void Render();
	void HandleInput(GLFWwindow* window, float deltaTime);

	Scene* GetCurrentScene() { return mCurrentScene; }
	void ChangeScene(int order);
	void ChangeScene(std::string sceneName);

	void AddScene(Scene* scene);
	void RemoveScene(int order);

	int GetTotalSceneAmount() { return scenes.size(); }

	std::vector<Scene*> scenes;
private:

	static SceneManager* instance;
	Scene* mCurrentScene;
	GLFWwindow* mWindow;
	SceneLevel1* tempLevel1;

	void SetupScenes()
	{
		AddScene((Scene*)new SceneLevel1(mWindow));
	}

};
