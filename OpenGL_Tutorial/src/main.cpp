#include <iostream>
#include <string>
#include <random>

#define _USE_MATH_DEFINES
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// User defined headers
#include "lib/stb_image/stb_image.h"
#include "inc/Color.h"
#include "inc/Shader.h"
#include "inc/ErrorHandler.h"
#include "inc/Buffers.h"
#include "inc/VertexArray.h"
#include "inc/Texture.h"
#include "inc/Camera.h"
#include "inc/TerrainGenerator.h"

#include "data/data.h"

// GLM defines
#include <glm/glm.hpp>						// vec2, vec3, vec4, radians
#include <glm/gtc/matrix_transform.hpp>		
#include <glm/gtc/type_ptr.hpp>

// Configurations
#include "inc/Configurations.h"

bool M_STATE = false;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		M_STATE = true;
	}
}

//Callback function to dynamically change the size of the rendering window.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Function to deal with inputs
void HandleInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// No Colour
// Assume it has 5 * sizeof(float) amount of data per vertices
struct Cube {
	glm::vec3 position;
	glm::vec3 rotationalAxis;
	float rotOffset;
	float speedOffset;
};

int main()
{
	// Initialise the GFLW
	glfwInit();

	// Configure the OpenGL Settings for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Generate a window object.
	//GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	//int xpos, ypos, windowWidth, windowHeight;
	//glfwGetMonitorWorkarea(monitor, &xpos, &ypos, &windowHeight, &windowWidth);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Boxes", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create the GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}

	/* TODO: Use the better error handling for versions higher than 4.3 */

	// Make window the main context in the current thread. 
	glfwMakeContextCurrent(window);

	//Intialise GLAD (retrieves the openGL library)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Configure OpenGL window.
	// Dynamically change the render window based on the window size.
	GLCALL(glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	/* OPENGL VALUE INITIALISATION
	* --------------------------------------
	*/

	RGB clr = HSV2RGB(98.0f, 0.68f, 0.68f);
	TerrainGenerator terrain(256, 1.0f / 32.0f, clr);

	/*+=====================================+
	* |										|
	* |			OPENGL CODE					|
	* |										|
	* +=====================================+
	*/
	{
		/* --- Generating OpenGL Objects --- */
		glEnable(GL_DEPTH_TEST);		// activate depth testing

		// Create Vertex Array Object (VAO)
		VertexArray VAO;

		// Create the shaders
		Shader shader("res/basic/vertex.glsl", "res/basic/fragment.glsl");

		/* --- Initialisation Code --- */
		// 2. copy our vertices array in a buffer for OpenGL to use
		VertexBuffer VBO(terrain.GetVertPtr(), terrain.GetVertSize(), GL_DYNAMIC_DRAW);

		//// 2a. copy our indices array in a buffer for OpenGL to use
		ElementBuffer EBO(terrain.GetIndicesPtr(), terrain.GetIndicesSize(), GL_DYNAMIC_DRAW);

		VertexAttribute attribs;
		attribs.Push<float>(3, GL_FALSE);	// vertex
		attribs.Push<float>(3, GL_FALSE);	// colour
		//attribs.Push<float>(2, GL_FALSE);	// texture coordinates

		VAO.AddBuffer(VBO, attribs);

		// Use the shader
		shader.use();

		/* Camera Initialisation and Vars */
		Camera camera(window, SPEED, SENSITIVITY);

		/* Coordinate System */
		// 1. Model Matrix
		glm::mat4 modelMat = glm::mat4(1.0f);

		// 2. View Matrix
		glm::mat4 viewMat;
		camera.UpdateViewMatrix(viewMat);

		// 3. Projection Matrix
		glm::mat4 projectionMat;
		projectionMat = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

		/* Other Variables */
		RGB bgColour = NormRGB(52, 152, 219);
		float initTime = (float)glfwGetTime();
		float dt = 0.0f;
		float inc = 360.0f;
		float rot = 0.0f;

		glfwSetKeyCallback(window, key_callback);

		//Render Window
		while (!glfwWindowShouldClose(window))
		{
			// Input Handling
			camera.HandleInputs(dt);
			camera.UpdateViewMatrix(viewMat);
			HandleInput(window);

			// Set the background colour and clear
			GLCALL(glClearColor(bgColour.R, bgColour.G, bgColour.B, 1.0f));
			GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
			
			// Modify Uniform
			if (M_STATE) {
				terrain.TerrainInit();
				VBO.Update(terrain.GetVertPtr(), terrain.GetVertSize(), GL_DYNAMIC_DRAW);
				EBO.Update(terrain.GetIndicesPtr(), terrain.GetIndicesSize(), GL_DYNAMIC_DRAW);
				M_STATE = false;
			}

			// Upate coordinate space
			shader.SetUniformMat4("model", glm::value_ptr(modelMat));
			shader.SetUniformMat4("view", glm::value_ptr(viewMat));
			shader.SetUniformMat4("projection", glm::value_ptr(projectionMat));

			shader.use();
			VAO.Bind();
			GLCALL(glDrawElements(GL_TRIANGLES, EBO.GetCount(), GL_UNSIGNED_INT, 0));
			
			// Update deltatime
			dt = (float)glfwGetTime() - initTime;
			initTime = (float)glfwGetTime();

			// check and call events and swap the buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

	}

	glfwTerminate();
}