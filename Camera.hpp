#pragma once

#include <SDL.h>
#include <glm/glm.hpp>
#include <bitset>

class Camera {
public:
	Camera();
	Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	~Camera();

	/// <summary>
	/// Getter of 'view matrix'
	/// </summary>
	/// <returns> 4×4 view matrix </returns>
	glm::mat4 getView() const;
	glm::mat4 getViewProj() const;
	glm::vec3 getPosition() const;

	void setSpeed(float speed);
	void setProjection(float angle, float ratio, float close, float far);
	void resize(size_t width, size_t height);

	void Update(float delta_time);

	void Camera::KeyEvent(const SDL_KeyboardEvent& ev, bool down);
	void Camera::MouseMoveEvent(const SDL_MouseMotionEvent& ev);

private:
	void setView(glm::vec3 eye, glm::vec3 at, glm::vec3 up);

	glm::vec3 m_eye;
	glm::vec3 m_at;
	glm::vec3 m_up;

	/// <summary>
	/// Vector from 'eye' to 'at'
	/// </summary>
	glm::vec3 m_forward_vec;
	glm::vec3 m_right_vec;
	float m_look_dist;

	float m_u, m_v;

	/// <summary>
	/// View matrix
	/// </summary>
	glm::mat4 m_view;
	glm::mat4 m_projection;

	float m_speed;
	std::bitset<4> m_direction;

	float m_FOV;
	float m_screen_ratio;
	float m_close;
	float m_far;
};

