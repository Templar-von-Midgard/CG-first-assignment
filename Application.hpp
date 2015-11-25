#pragma once

#include <GL\glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "Camera.hpp"

class Application {
public:
	Application();
	~Application();

	/// <summary>
	/// Initializes the application
	/// </summary>
	bool Init();

	/// <summary>
	/// Renders the scene
	/// </summary>
	void Render();

	/// <summary>
	/// Updates the scene
	/// </summary>
	void Update();

	void KeyDown(const SDL_KeyboardEvent& key);
	void KeyUp(const SDL_KeyboardEvent& key);
	void MouseMove(const SDL_MouseMotionEvent& mouse);
	void Resize(Sint32, Sint32);

private:
	/// <summary>
	/// Clears up the aquired resources
	/// </summary>
	void Clear();

	GLuint m_VAO_ID[2];
	GLuint m_VBO_ID[2];
	GLuint m_IB_ID[2];

	GLuint m_program_ID;

	Camera m_cam;
	glm::mat4 m_world;

	GLuint m_loc_MVP;

	float m_scaling;
	float m_rotation;

	bool draw_x, draw_y, draw_z, scale;

	struct Vertex {
		glm::vec3 position, colour;
	};

	void compute_cylinders(Vertex vertices[]);
	void compute_circles(Vertex vertices[]);
	void RenderCylinder(glm::mat4 MVP, glm::vec3 xyz);
};

