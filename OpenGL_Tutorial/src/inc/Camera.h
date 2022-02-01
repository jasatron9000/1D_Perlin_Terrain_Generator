#pragma once

#include <iostream>

#include <GLFW/glfw3.h>
#include "Configurations.h"
#include "ErrorHandler.h"

// GLM defines
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>		
#include <glm/gtc/type_ptr.hpp>


// TODO : Make this a singleton to avoid generating two different cameras
class Camera
{
private:
	GLFWwindow* m_WindowPtr;
	float m_Speed;
	float m_Sensitivity;

	// Rotation Intiailialsiation
	float m_lastX, m_lastY;
	float m_Yaw, m_Pitch;
	bool m_firstMouse;

	// Camera Vectors
	glm::vec3 m_camPos, m_camFront, m_camUp;

	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);

	// Handle Inputs
	void HandleMovement(float dt);
	void HandleMouseInputs(double xpos, double ypos);

public:
	Camera(GLFWwindow* window, float speed, float sensitivity);

	// Handle Inputs
	void HandleInputs(float dt);
	void UpdateViewMatrix(glm::mat4 &viewMatrix);

	// set the speed and sensitivity
	inline void SetSpeed(float speed) { this->m_Speed; }
	inline void SetSensitivity(float sensitivity) { this->m_Sensitivity; }

};