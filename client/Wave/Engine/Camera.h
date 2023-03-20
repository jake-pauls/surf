#pragma once

#include <glm/glm.hpp>

namespace wv
{
	class Camera
	{
	public:
		Camera();
		~Camera() = default;

	private:
		void Update();

	public:
		void Orbit(float xOffset, float yOffset);

		void Zoom(float yOffset);

		void SetScreenDimensions(float width, float height);

		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix() const;

	private:
		float m_ScreenWidth = 800.0f;
		float m_ScreenHeight = 600.0f;

		float m_CameraFoV = 90.0f;
		float m_CameraDistance = 10.0f;
		float m_CameraXAngle = 90.0f;
		float m_CameraYAngle = 30.0f;

		float m_MouseSensitivity = 0.3f;
		float m_ZoomSensitivity = 0.5f;
		float m_FovSensitivity = 0.4f;

		glm::vec3 m_CameraPosition = glm::vec3(0.0f, 0.0f, -2.0f);
		glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	};
}