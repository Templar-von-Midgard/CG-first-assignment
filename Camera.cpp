#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Camera::Camera()
	: m_eye(5,5,5), m_at(5,0,0), m_up(0,1,0), m_forward_vec(glm::normalize(m_at - m_eye))
	, m_projection(glm::perspective(45.0f, 640.0f/480.0f, 1.0f, 1000.0f)), m_speed(16.0f) {
	setView(m_eye, m_at, m_up);
}

Camera::Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
	: m_forward_vec(glm::normalize(m_at - m_eye))
	, m_speed(16.0f) {
	setView(eye, at, up);
}

Camera::~Camera() {}

glm::mat4 Camera::getView() const {
	return m_view;
}

glm::mat4 Camera::getViewProj() const {
	return m_projection*m_view;
}

void Camera::setSpeed(float speed) {
	m_speed = speed;
}

void Camera::Update(float dt) {
	static glm::vec3 dir;
	dir = m_forward_vec*static_cast<float>( m_direction[0] - m_direction[1] )
		+ m_right_vec*static_cast<float>( m_direction[2] - m_direction[3] );
	m_eye += dir*m_speed*dt;
	m_at  += dir*m_speed*dt;

	m_view = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::setProjection(float angle, float ratio, float close, float far) {
	m_FOV = angle;
	m_screen_ratio = ratio;
	m_close = close;
	m_far = far;
	m_projection = glm::perspective(m_FOV, m_screen_ratio, m_close, m_far);
}

void Camera::resize(size_t width, size_t height) {
	m_screen_ratio = width / static_cast<float>( height );
	m_projection = glm::perspective(m_FOV, m_screen_ratio, m_close, m_far);
}

void Camera::KeyEvent(const SDL_KeyboardEvent& ev, bool down) {
	switch( ev.keysym.sym ) {
	case SDLK_w:
		m_direction[0] = down;
		break;
	case SDLK_s:
		m_direction[1] = down;
		break;
	case SDLK_a:
		m_direction[2] = down;
		break;
	case SDLK_d:
		m_direction[3] = down;
		break;
	default:
		break;
	}
}

void Camera::MouseMoveEvent(const SDL_MouseMotionEvent& ev) {
	std::cout << m_u << ',' << m_v << '\n';
	if( !( ev.state & SDL_BUTTON_LMASK ) ) return;

	m_u += ev.xrel / 100.0f;
	m_v += ev.yrel / 100.0f;

	m_at = m_eye + m_look_dist*glm::vec3(cosf(m_v)*sinf(m_u), sinf(m_v), cosf(m_u)*cosf(m_v));

	m_forward_vec = glm::normalize(m_at - m_eye);
	m_right_vec = glm::normalize(glm::cross(m_up, m_forward_vec));

}

void Camera::setView(glm::vec3 eye, glm::vec3 at, glm::vec3 up) {
	m_eye = eye;
	m_at = at;
	m_up = up;

	m_forward_vec = glm::normalize(m_at - m_eye);
	m_right_vec = glm::normalize(glm::cross(m_up, m_forward_vec));

	m_look_dist = glm::length(m_at - m_eye);

	m_view = glm::lookAt(m_eye, m_at, m_up);

	float dot = glm::dot(m_forward_vec, m_up);
	glm::vec3 vec = glm::normalize(m_forward_vec - dot*m_up);
	float A = glm::length(m_forward_vec - dot*m_up);
	m_u = 2.0f*M_PI - acosf(vec.z);
	m_v = atanf(dot/A);
}