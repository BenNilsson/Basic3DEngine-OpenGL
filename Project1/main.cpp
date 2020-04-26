#include <glad/glad.h>
#include <KHR/khrplatform.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H


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

// Text -------------------
struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
GLuint VAO, VBO;

void RenderText(Shader& shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

// Skybox -------------------
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
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);


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
	
	//Tell GLFW to capture mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// GLAD: Load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Camera
	Camera::GetInstance();

	Camera::GetInstance()->SCREEN_WIDTH = mode->width;
	Camera::GetInstance()->SCREEN_HEIGHT = mode->height;
	
	// TEXT -------------------------------
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Shader
	Shader shader("Shaders/text.vs", "Shaders/text.frag");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(1920), 0.0f, static_cast<GLfloat>(1080));
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// Setup FreeType
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "Error:: Could not init FreeType" << std::endl;

	// Load Font 
	FT_Face face;
	if (FT_New_Face(ft, "Fonts/Biko_Regular.otf", 0, &face))
		std::cout << "Error: Failed to load font" << std::endl;

	// Set size
	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load characters
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "Error: Failed to load glyph" << std::endl;
			continue;
		}
		// Texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set Texture Options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Store Character
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


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
			
			RenderText(shader, "AmateurEngine2020 - Penguin Scene", 25.0f, 25.0f, 0.5f, glm::vec3(0.2f, 0.8f, 0.2f));
			

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

void RenderText(Shader& shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	glDisable(GL_DEPTH_TEST);
	// Shader
	shader.use();
	glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// Iterate through characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render Glyph Texture
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Advance cursors
		x += (ch.Advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
}