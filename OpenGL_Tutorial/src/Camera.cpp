#include "inc\Camera.h"

void Camera::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Camera* cameraObj = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));

	if (cameraObj) {
		cameraObj->HandleMouseInputs(xpos, ypos);
	}
}

void Camera::HandleMovement(float dt)
{
	float velocity = m_Speed * dt;

	if (glfwGetKey(m_WindowPtr, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		velocity *= 5.0f;

	// Move forward
	if (glfwGetKey(m_WindowPtr, GLFW_KEY_W) == GLFW_PRESS)
		m_camPos += velocity * m_camFront;

	// Move backward
	if (glfwGetKey(m_WindowPtr, GLFW_KEY_S) == GLFW_PRESS)
		m_camPos -= velocity * m_camFront;

	// Move Left
	if (glfwGetKey(m_WindowPtr, GLFW_KEY_A) == GLFW_PRESS)
		m_camPos -= velocity * glm::normalize(glm::cross(m_camFront, m_camUp));

	// Move Right
	if (glfwGetKey(m_WindowPtr, GLFW_KEY_D) == GLFW_PRESS)
		m_camPos += velocity * glm::normalize(glm::cross(m_camFront, m_camUp));
}

void Camera::HandleMouseInputs(double xpos, double ypos)
{
	// Remove issue with the bugged value
	if (m_firstMouse) {
		m_lastX = (float)xpos;
		m_lastY = (float)ypos;
		m_firstMouse = false;
	}

	// Calcualte the offsets and update last values
	float xoffset = (float)xpos - m_lastX;
	float yoffset = m_lastY - (float)ypos;
	m_lastX = (float)xpos;
	m_lastY = (float)ypos;

	// Adjust the offsets based on sensitivity
	xoffset *= m_Sensitivity;
	yoffset *= m_Sensitivity;

	// Get Yaw and Pitch
	m_Yaw += xoffset;
	m_Pitch += yoffset;

	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;

	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;

	//Calcualte 3D vector
	glm::vec3 direction(
		cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),		// X
		sin(glm::radians(m_Pitch)),									// Y
		sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch))		// Z
	);
	m_camFront = glm::normalize(direction);
}

Camera::Camera(GLFWwindow* window, float speed, float sensitivity)
	: m_WindowPtr(window), m_Speed(speed), m_Sensitivity(sensitivity)
{
	// Initialise the camera vectors
	m_camPos = glm::vec3(0.0f, 0.0f, 3.0f);
	m_camFront = glm::vec3(0.0f, 0.0f, -1.0f);
	m_camUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// Initialise the Mouse inputs
	m_lastX = (float)WINDOW_WIDTH / 2.0f;
	m_lastY = (float)WINDOW_HEIGHT / 2.0f;
	m_firstMouse = true;

	// Initialise the yaw and pitch angles
	m_Yaw = -90.0f;
	m_Pitch = 0.0f;

	// Set the Mouse Callback
	glfwSetInputMode(m_WindowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(m_WindowPtr, reinterpret_cast<void*>(this)); // make the current object the pointer
	glfwSetCursorPosCallback(m_WindowPtr, MouseCallback);

}

void Camera::HandleInputs(float dt)
{
	HandleMovement(dt);
}

void Camera::UpdateViewMatrix(glm::mat4& viewMatrix)
{
	viewMatrix = glm::lookAt(
		m_camPos,
		m_camPos + m_camFront,
		m_camUp
	);
}
