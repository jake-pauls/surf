#include "Camera.h"

#include <glm/gtx/transform.hpp>

wv::Camera::Camera()
{
	Update();
}

void wv::Camera::Update()
{
	m_CameraPosition = glm::vec3(
		glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch)),
		glm::sin(glm::radians(m_Pitch)),
		glm::sin(glm::radians(m_Yaw) * glm::cos(glm::radians(m_Pitch)))
	);

	m_CameraPosition *= glm::vec3(m_CameraDistance);

	glm::vec3 frontVector = glm::normalize(m_CameraPosition) * glm::vec3(-1.0f);
	glm::vec3 rightVector = glm::normalize(glm::cross(frontVector, glm::vec3(0.0f, 1.0f, 0.0f)));

	m_CameraUp = glm::normalize(glm::cross(rightVector, frontVector));
}

void wv::Camera::Orbit(float xOffset, float yOffset)
{
	m_Yaw += xOffset * m_MouseSensitivity;
	m_Pitch -= yOffset * m_MouseSensitivity;

	Update();
}

void wv::Camera::Zoom(float yOffset)
{
	m_CameraDistance -= yOffset * m_ZoomSensitivity;

	if (m_CameraDistance < 0.5f)
		m_CameraDistance = 0.5f;
	if (m_CameraDistance > 45.0f)
		m_CameraDistance = 45.0f;

	Update();
}

void wv::Camera::SetScreenDimensions(float width, float height)
{
	if (width != m_ScreenWidth)
		m_ScreenWidth = width;

	if (height != m_ScreenHeight)
		m_ScreenHeight = height;
}

glm::mat4 wv::Camera::GetViewMatrix() const
{
	return glm::lookAt(m_CameraPosition, glm::vec3(0.0f), m_CameraUp);
}

glm::mat4 wv::Camera::GetProjectionMatrix() const
{
	glm::mat4 projection = glm::perspective(m_CameraFoV, m_ScreenWidth / m_ScreenHeight, 0.1f, 100.0f);
	projection[1][1] *= -1;

	return projection;
}
