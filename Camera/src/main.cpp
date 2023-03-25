#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "awesomeface.h"
#include "camera.h"
#include "ebo.h"
#include "shader.h"
#include "vao.h"
#include "Texture2D.h"


//Defining variables inside unnamed namespace will make them global only to main.cpp
namespace
{
	constexpr int SCREEN_WIDTH = 800;
	constexpr int SCREEN_HEIGHT = 600;
	Camera camera(FPS);
	float deltaTime = 0.0f; // Time between current frame and previous frame
	float previousFrame = 0.0f; // Time of previous frame

	//These values need to be adjusted
	float lastMouseX = 800.0f;
	float lastMouseY = 300.0f;

	bool firstMouse = true;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	const float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - previousFrame;
	previousFrame = currentFrame;
	camera.speed = 2.0f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(MOVE_FORWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(MOVE_BACKWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(STRAFE_LEFT);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(STRAFE_RIGHT);
	}
}

void LoadVertexDataIntoGPUMemory(VAO& vao, float vertices[], int verticesSize, unsigned int indices[], int indicesSize)
{
	vao.Bind();

	//Generate and bind the VBO
	VBO vbo = VBO(vertices, verticesSize);

	//Generate and bind the EBO
	// EBO ebo = EBO(indices, indicesSize);

	int stride = 5 * sizeof(float);

	int vertexLayout = 0;
	int vertexOffset = 0;
	vao.LinkVBO(vbo, vertexLayout, stride, vertexOffset);

	int textureLayout = 1;
	int textureOffset = 3 * sizeof(float);
	vao.LinkVBO(vbo, textureLayout, stride, textureOffset, true);
}


void InitializeGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* CreateWindow(int screenWidth, int screenHeight, const char* title)
{
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, title, nullptr, nullptr);
	return window;
}

void SetWindowRenderProperties(GLFWwindow* window)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//State setting function
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			//State using function
}


bool LoadGlad()
{
	// GLAD manages function pointers for OpenGL so we
	// want to initialize GLAD before we call any OpenGL function
	return gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
}


void DoTransformations(Shader& shader, glm::vec3 cubePositions[], int i)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, cubePositions[i]);
	model = glm::rotate(model, (i%3 && i!=1)? 0 : (static_cast<float>(i) + 1.0f) * sin(static_cast<float>(glfwGetTime())), glm::vec3(0.5f, 1.0f, 0.0f));

	camera.UpdateCameraLookAtMatrix();
	glm::mat4 view = camera.view;

	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
		static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f,
		100.0f);

	shader.SetUniformMatrix4fv("model", model);
	shader.SetUniformMatrix4fv("view", view);
	shader.SetUniformMatrix4fv("projection", projection);
}

void Run(GLFWwindow* window)
{
	const char* vertexFile = "resources/shaders/vertex.glsl";
	const char* fragmentFile = "resources/shaders/fragment.glsl";
	// const char* triangleTwoFragmentFile = "resources/shaders/triangleTwoFragment.glsl";

	Shader woodenContainerShader = Shader(vertexFile, fragmentFile);
	// Shader triangleTwoShader = Shader(vertexFile, triangleTwoFragmentFile);

	//Translation vectors for different cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	// float vertices[] = {
// 	//vertex			//color			    //texture coords
// 	-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,		//bottom left
// 	-0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,   //top left
// 	0.5f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,  //top right
// 	0.5f, -0.5f, 0.0f,	0.5f, 0.5f, 0.2f,	1.0f, 0.0f,   //bottom right
// };


	float vertices[] = {
		-0.5f,	-0.5f,	-0.5f,		0.0f,	0.0f,
		0.5f,	-0.5f,	-0.5f,		1.0f,	0.0f,
		0.5f,	0.5f,	-0.5f,		1.0f,	1.0f,
		0.5f,	0.5f,	-0.5f,		1.0f,	1.0f,
		-0.5f,	0.5f,	-0.5f,		0.0f,	1.0f,
		-0.5f,	-0.5f,	-0.5f,		0.0f,	0.0f,

		-0.5f,	-0.5f,	0.5f,		0.0f,	0.0f,
		0.5f,	-0.5f,	0.5f,		1.0f,	0.0f,
		0.5f,	0.5f,	0.5f,		1.0f,	1.0f,
		0.5f,	0.5f,	0.5f,		1.0f,	1.0f,
		-0.5f,	0.5f,	0.5f,		0.0f,	1.0f,
		-0.5f,	-0.5f,	0.5f,		0.0f,	0.0f,

		-0.5f,	0.5f,	0.5f,		1.0f,	0.0f,
		-0.5f,	0.5f,	-0.5f,		1.0f,	1.0f,
		-0.5f,	-0.5f,	-0.5f,		0.0f,	1.0f,
		-0.5f,	-0.5f,	-0.5f,		0.0f,	1.0f,
		-0.5f,	-0.5f,	0.5f,		0.0f,	0.0f,
		-0.5f,	0.5f,	0.5f,		1.0f,	0.0f,

		0.5f,	0.5f,	0.5f,		1.0f,	0.0f,
		0.5f,	0.5f,	-0.5f,		1.0f,	1.0f,
		0.5f,	-0.5f,	-0.5f,		0.0f,	1.0f,
		0.5f,	-0.5f,	-0.5f,		0.0f,	1.0f,
		0.5f,	-0.5f,	0.5f,		0.0f,	0.0f,
		0.5f,	0.5f,	0.5f,		1.0f,	0.0f,

		-0.5f,	-0.5f,	-0.5f,		0.0f,	1.0f,
		0.5f,	-0.5f,	-0.5f,		1.0f,	1.0f,
		0.5f,	-0.5f,	0.5f,		1.0f,	0.0f,
		0.5f,	-0.5f,	0.5f,		1.0f,	0.0f,
		-0.5f,	-0.5f,	0.5f,		0.0f,	0.0f,
		-0.5f,	-0.5f,	-0.5f,		0.0f,	1.0f,

		-0.5f,	0.5f,	-0.5f,		0.0f,	1.0f,
		0.5f,	0.5f,	-0.5f,		1.0f,	1.0f,
		0.5f,	0.5f,	0.5f,		1.0f,	0.0f,
		0.5f,	0.5f,	0.5f,		1.0f,	0.0f,
		-0.5f,	0.5f,	0.5f,		0.0f,	0.0f,
		-0.5f,	0.5f,	-0.5f,		0.0f,	1.0f
	};


	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3,
	};

	VAO vao;
	LoadVertexDataIntoGPUMemory(vao, vertices, sizeof(vertices), indices, sizeof(indices));
	Texture2D texture2d = Texture2D("resources/textures/container.jpg");
	// Texture2D awesomefaceTexture = Texture2D("resources/textures/awesomeface.png");
	// unsigned int awesomeFaceTextureId = SetupAwesomeFace();

	while (!glfwWindowShouldClose(window))
	{
		SetWindowRenderProperties(window);

		// input
		processInput(window);

		//rendering commands here

		woodenContainerShader.SetUniformi("texture1", 0);
		woodenContainerShader.SetUniformi("texture2", 1);
		glUseProgram(woodenContainerShader.ID);

		//Don't forget to bind the textures before calling glDrawElements
		texture2d.Bind(GL_TEXTURE0);
		// awesomefaceTexture.Bind(GL_TEXTURE1);
		// BindAwesomeFaceTexture(awesomeFaceTextureId, GL_TEXTURE1);


		vao.Bind(); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

		//TODO: Upper limit of for loop is dependent on cubePositions array, improve this
		for (int i = 0; i < 10; ++i)
		{
			DoTransformations(woodenContainerShader, cubePositions, i);
			//TODO: Use EBO for better performance
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	//(xpos, ypos) is the coordinate of the mouse pointer
	camera.ProcessMouseMovement(xpos, ypos, lastMouseX, lastMouseY, firstMouse);
}

int main()
{
	InitializeGLFW();

	GLFWwindow* window = CreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "FPS Camera");
	if (!window)
	{
		std::cout << "Failed to initialize window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!LoadGlad())
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, MouseCallback);

	//This will ensure that the mouse becomes hidden and gets captured when the window is on focus.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Run(window);

	glfwTerminate();

	return 0;
}
