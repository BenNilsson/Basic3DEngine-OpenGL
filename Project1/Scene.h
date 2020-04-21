#pragma once

#include "GameObject.h"

#include <vector>
#include <string>

class Scene
{
public:
	Scene(std::string name, GLFWwindow* window) { mName = name; mWindow = window; }
	~Scene() { }

	virtual void Render() { }
	virtual void Update(float deltaTime) { }
	virtual void HandleInputs(GLFWwindow* window, float deltaTime) { }
	
	// Callbacks
	virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos) {}
	virtual void scroll_callback(GLFWwindow* window, double offset, double yoffset) {}
	virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {}

	std::string& GetName() { return mName; }
	int& GetOrder() { return mOrder; }
	int SetOrder(int order) { mOrder = order; }

	void AddGameObject(GameObject* object);
	void RemoveGameObject(GameObject* object);
protected:

	std::vector<GameObject*> objects;

private:

	GLFWwindow* mWindow;
	std::string mName;
	int mOrder;
};

