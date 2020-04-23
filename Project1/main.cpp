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
#include "stb_image.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double offset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void HandleInputs(GLFWwindow* window);

// Skybox
std::vector<std::string> mSkyboxFaces;
Shader* mSkyboxShader;
unsigned int mSkyboxTextures;

// Skybox Vertices
float skyboxVertices[] = {
	-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
};

unsigned int LoadCubemap(std::vector<std::string> faces);
unsigned int skyboxVAO, skyboxVBO;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Setup a timer with a fixedStep (time between calls), this will result in ~60 fps
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
	glfwWindowHint(GLFW_SAMPLES, 4);

	//SCREEN_WIDTH = mode->width;
	//SCREEN_HEIGHT = mode->height;

	// GLFW Create Window
	// For fullscreen: glfwGetPrimaryMonitor()
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "AmateurEngine 2020.exe", NULL, NULL);
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
	
	// Backface culling
	glEnable(GL_CULL_FACE);

	// Anti-aliasing
	glEnable(GL_MULTISAMPLE);


	// Skybox ----------

	mSkyboxShader = new Shader("Shaders/skybox.vs", "Shaders/skybox.fs");
	// Skybox VAO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	mSkyboxFaces.push_back("Textures/Skyboxv2/right.png");
	mSkyboxFaces.push_back("Textures/Skyboxv2/left.png");
	mSkyboxFaces.push_back("Textures/Skyboxv2/top.png");
	mSkyboxFaces.push_back("Textures/Skyboxv2/bottom.png");
	mSkyboxFaces.push_back("Textures/Skyboxv2/front.png");
	mSkyboxFaces.push_back("Textures/Skyboxv2/back.png");

	mSkyboxTextures = LoadCubemap(mSkyboxFaces);

	mSkyboxShader->use();
	mSkyboxShader->setInt("skybox", 0);

	// Initialise PhysicsEngineD
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

			// Update PhysicsEngine
			PhysicsEngine::GetInstance()->Simulate(deltaTime);
			PhysicsEngine::GetInstance()->ApplyForces(deltaTime);
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
			
			// ---------- Skybox
			mSkyboxShader->use();
			glm::mat4 view = Camera::GetInstance()->GetViewMatrix();
			view = glm::mat4(glm::mat3(Camera::GetInstance()->GetViewMatrix()));
			glm::mat4 projection = glm::perspective(glm::radians(Camera::GetInstance()->Zoom), (float)Camera::GetInstance()->SCREEN_WIDTH / (float)Camera::GetInstance()->SCREEN_HEIGHT, 0.1f, 100.0f);
			mSkyboxShader->setMat4("view", view);
			mSkyboxShader->setMat4("projection", projection);

			// Draw Skybox

			// Change depth function
			glDepthFunc(GL_LEQUAL);
			glBindVertexArray(skyboxVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, mSkyboxTextures);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			// Set depth function back to default
			glDepthFunc(GL_LESS);
			
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

unsigned int LoadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap failed loading: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
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
