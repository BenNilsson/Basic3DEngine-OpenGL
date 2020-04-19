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

#include <iostream>

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double offset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void HandleInputs(GLFWwindow* window);

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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

		// Update & Render current scene if they exist
		if (SceneManager::GetInstance()->GetCurrentScene() != nullptr)
		{
			SceneManager::GetInstance()->HandleInput(window, deltaTime);
			SceneManager::GetInstance()->Update(deltaTime);
			SceneManager::GetInstance()->Render();
		}

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

	/*
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
		*/
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
}

void scroll_callback(GLFWwindow* window, double offset, double yoffset)
{
	Camera::GetInstance()->ProcessMouseScroll(yoffset);
}
