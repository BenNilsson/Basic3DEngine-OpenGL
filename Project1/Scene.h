#pragma once
#include <GLFW/glfw3.h>

#include <string>

class Scene
{
public:
	Scene(std::string name, GLFWwindow* window) { mName = name; mWindow = window; }
	~Scene() { }

	virtual void Render() { }
	virtual void Update(float deltaTime) { }
	virtual void HandleInputs(GLFWwindow* window, float deltaTime) { }

	std::string& GetName() { return mName; }
	int& GetOrder() { return mOrder; }
	int SetOrder(int order) { mOrder = order; }

private:
	GLFWwindow* mWindow;
	std::string mName;
	int mOrder;
};

