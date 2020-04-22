#include <glad/glad.h>
#include <KHR/khrplatform.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SceneManager.h"
#include "Shader.h"
#include "Camera.h"
#include "GameObject.h"
#include "PhysicsEngine.h"

#include <iostream>

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double offset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void HandleInputs(GLFWwindow* window);

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float timer = 0.0f;
float fixedStep = 0.016f;

int main(void)
{
	// GLFW Initialise and configure
	glfwInit();

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	//SCREEN_WIDTH = mode->width;
	//SCREEN_HEIGHT = mode->height;

	// GLFW Create Window
	// For fullscreen: glfwGetPrimaryMonitor()
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Best Window You'll Ever See 2020", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLAD: Load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Enable Z-Buffer (Depth Testing)
	glEnable(GL_DEPTH_TEST);

	// Initialise PhysicsEngine
	PhysicsEngine::GetInstance();

	// Camera
	Camera::GetInstance();

	// Set up scene manager. This will create the firt instance of the sceene manager
	SceneManager::GetInstance();

	// Load level 1
	SceneManager::GetInstance()->ChangeScene(0);

	// Game Loop
	while (!glfwWindowShouldClose(window))
	{
		// Timings
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Clear background color and buffers
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Input
		HandleInputs(window);

		while (timer > fixedStep)
		{
			/*
			Current problem:
			These methods below and ran so often that, for instance, any form of collision response is called so rapidly that it "inverts"
			itself. For instance, the change in velocity in HandleCollisions will not take place.
			I assume that calling physics should happen in regards to a sort of fixedUpdate method? Like Unity, need to research it more
			*/
			// Update PhysicsEngine
			PhysicsEngine::GetInstance()->Simulate(deltaTime);
			PhysicsEngine::GetInstance()->HandleCollisions();
			if (SceneManager::GetInstance()->GetCurrentScene() != nullptr)
			{
				SceneManager::GetInstance()->FixedUpdate(deltaTime);
			}

			timer -= fixedStep;
		}

		if (SceneManager::GetInstance()->GetCurrentScene() != nullptr)
		{
			SceneManager::GetInstance()->HandleInput(window, deltaTime);
			SceneManager::GetInstance()->Update(deltaTime);
			SceneManager::GetInstance()->Render();
		}
		
		timer += deltaTime;
		

		// GLFW: Swap buffer and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	

	// Terminate window and return
	glfwTerminate();
	return 0;
}

void HandleInputs(GLFWwindow* window)
{
	// Check if the user hits escape
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// Close window
		glfwSetWindowShouldClose(window, true);
	}

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//Mouse Movement Callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (Camera::GetInstance()->firstMouse)
	{
		Camera::GetInstance()->lastX = xpos;
		Camera::GetInstance()->lastY = ypos;
		Camera::GetInstance()->firstMouse = false;
	}

	float xoffset = xpos - Camera::GetInstance()->lastX;
	float yoffset = Camera::GetInstance()->lastY - ypos; //Y-axis reversed

	Camera::GetInstance()->lastX = xpos;
	Camera::GetInstance()->lastY = ypos;

	Camera::GetInstance()->ProcessMouseMovement(xoffset, yoffset);

	// Scene callback
	if (SceneManager::GetInstance()->GetCurrentScene() != nullptr)
	{
		SceneManager::GetInstance()->GetCurrentScene()->mouse_callback(window, xpos, ypos);
	}
}

void scroll_callback(GLFWwindow* window, double offset, double yoffset)
{
	Camera::GetInstance()->ProcessMouseScroll(yoffset);

	// Scene callback
	if (SceneManager::GetInstance()->GetCurrentScene() != nullptr)
	{
		SceneManager::GetInstance()->GetCurrentScene()->scroll_callback(window, offset, yoffset);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Scene callback
	if (SceneManager::GetInstance()->GetCurrentScene() != nullptr)
	{
		SceneManager::GetInstance()->GetCurrentScene()->key_callback(window, key, scancode, action, mods);
	}
}
