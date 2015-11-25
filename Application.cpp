#include "Application.hpp"
#include <cmath>
#include "GLUtils.hpp"

#define N 100
#define M 1
#define radius 2.0f
#define height 2.0f

glm::vec3 getUV(float theta, float z) {
	theta *= 2.0f*M_PI;
	float x = radius * cosf(theta), y = radius * sinf(theta);
	return glm::vec3(x, y, z);
}

void Application::compute_cylinders(Vertex vertices[]) {
	for( size_t i = 0; i <= N; ++i ) {
		for( size_t j = 0; j <= M; ++j ) {
			float theta = i / static_cast<float>( N ), z = height*j / static_cast<float>( M );
			vertices[i + j*( N + 1 )].position = getUV(theta, z);
			vertices[i + j*( N + 1 )].colour = glm::normalize(vertices[i + j*( N + 1 )].position);
		}
	}
	/*vertices[( N + 1 )*( M + 1 )].position = glm::vec3(height,0.0f,0.0f);
	vertices[( N + 1 )*( M + 1 )].colour = vertices[( N + 1 )*( M + 1 )].position;
	vertices[( N + 1 )*( M + 1 ) + 1].position = glm::vec3(height,0.0f,0.0f);
	vertices[( N + 1 )*( M + 1 ) + 1].colour = glm::normalize(vertices[( N + 1 )*( M + 1 ) + 1].position);*/
}

void Application::compute_circles(Vertex vertices[]) {
	for( size_t i = 0; i <= N; ++i ) {
		float t = 2.0f*M_PI * i/static_cast<float>(N);
		vertices[i].position = glm::vec3(0.0f, radius * sinf(t), radius * cosf(t));
		vertices[i].colour = glm::normalize(vertices[i].position);
	}
}

Application::Application() 
	: m_program_ID(0), m_scaling(1.0f), m_rotation(0.0f), scale(false) {
	m_VAO_ID[0] = 0;
	m_VAO_ID[1] = 0;
	m_VBO_ID[0] = 0;
	m_VBO_ID[1] = 0;
	m_IB_ID[0] = 0;
	m_IB_ID[1] = 0;

	m_cam.setProjection(45.0f, 640.0f/480.0f, 1.0f, 10000.0f);
}


Application::~Application() {
	Clear();
}


bool Application::Init() {
	glClearColor(0.100f, 0.200f, 0.250f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	// =================================================================
	// ================== Generate the Cylinder ========================
	// =================================================================

	Vertex cylinder_vertices[( N + 1 )*( M + 1 )];
	compute_cylinders(cylinder_vertices);

	GLushort cylinder_indices[3 * 2 * ( N )*( M )];
	for( size_t i = 0; i < N; ++i ) {
		for( size_t j = 0; j < M; ++j ) {
			cylinder_indices[6 * i + j * 3 * 2 * N + 0] = i + j        *( N + 1 );
			cylinder_indices[6 * i + j * 3 * 2 * N + 1] = i + 1 + j        *( N + 1 );
			cylinder_indices[6 * i + j * 3 * 2 * N + 2] = i + ( j + 1 )*( N + 1 );
			cylinder_indices[6 * i + j * 3 * 2 * N + 3] = i + 1 + j        *( N + 1 );
			cylinder_indices[6 * i + j * 3 * 2 * N + 4] = i + 1 + ( j + 1 )*( N + 1 );
			cylinder_indices[6 * i + j * 3 * 2 * N + 5] = i + ( j + 1 )*( N + 1 );
		}
	}

	// =================================================================
	// ================== Initialize buffers for Cylinder ==============
	// =================================================================
	glGenVertexArrays(1, &m_VAO_ID[0]);
	glBindVertexArray(m_VAO_ID[0]);

	glGenBuffers(1, &m_VBO_ID[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ID[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cylinder_vertices), cylinder_vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));

	glGenBuffers(1, &m_IB_ID[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IB_ID[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cylinder_indices), cylinder_indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// =================================================================
	// ================== Generate the Circle ==========================
	// =================================================================

	Vertex circle_vertices[N+1];
	compute_circles(circle_vertices);

	GLushort circle_indices[3*2*N];
	/*for( size_t i = 0; i < N; i++ ) {
		circle_indices[6 * i + 0] = i;
		circle_indices[6 * i + 1] = i+1;
		circle_indices[6 * i + 2] = i;
		circle_indices[6 * i + 3] = i;
		circle_indices[6 * i + 4] = i;
		circle_indices[6 * i + 5] = i;
	}*/
	for( size_t i = 0; i < N; ++i ) {
		for( size_t j = 0; j < M; ++j ) {
			circle_indices[6 * i + j * 3 * 2 * N + 0] = i + j        *( N + 1 );
			circle_indices[6 * i + j * 3 * 2 * N + 1] = i + 1 + j        *( N + 1 );
			circle_indices[6 * i + j * 3 * 2 * N + 2] = i + ( j + 1 )*( N + 1 );
			circle_indices[6 * i + j * 3 * 2 * N + 3] = i + 1 + j        *( N + 1 );
			circle_indices[6 * i + j * 3 * 2 * N + 4] = i + 1 + ( j + 1 )*( N + 1 );
			circle_indices[6 * i + j * 3 * 2 * N + 5] = i + ( j + 1 )*( N + 1 );
		}
	}
	
	// =================================================================
	// ================== Initialize buffers for Circle ================
	// =================================================================
	glGenVertexArrays(1, &m_VAO_ID[1]);
	glBindVertexArray(m_VAO_ID[1]);

	glGenBuffers(1, &m_VBO_ID[1]);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ID[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circle_vertices), circle_vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)( sizeof(glm::vec3) ));

	glGenBuffers(1, &m_IB_ID[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IB_ID[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(circle_indices), circle_indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	// =================================================================
	// ================== Create the shader program ====================
	// =================================================================
	GLuint vertex_shader = loadShader(GL_VERTEX_SHADER, "myVert.vert");
	GLuint fragment_shader = loadShader(GL_FRAGMENT_SHADER, "myFrag.frag");

	m_program_ID = glCreateProgram();

	glAttachShader(m_program_ID, vertex_shader);
	glAttachShader(m_program_ID, fragment_shader);

	glBindAttribLocation(m_program_ID, 0, "vs_in_pos");
	glBindAttribLocation(m_program_ID, 1, "vs_in_col");
	glLinkProgram(m_program_ID);

	// 'uniform' variables of the shader:
	m_loc_MVP = glGetUniformLocation(m_program_ID, "MVP");

	// TODO: Hiba ellenörzés

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// =================================================================
	// =========== Create the projection and view matrices =============
	// =================================================================

	return true;
}

void Application::RenderCylinder(glm::mat4 MVP, glm::vec3 xyz) {
	// Translate by 'xyz'
	MVP = glm::translate(MVP, xyz);

	// Rotate by 90° (to make the cylinder lie on the YZ axis)
	MVP = glm::rotate(MVP, 90.0f, 0.0f, 1.0f, 0.0f);
	glUniformMatrix4fv(m_loc_MVP, 1, GL_FALSE, &( MVP[0][0] )); // Send to the VGA

	// Draw surface
	glBindVertexArray(m_VAO_ID[0]);
	glDrawElements(GL_TRIANGLES, 3 * 2 * N*M, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	// Discard changes
	MVP = glm::rotate(MVP, -90.0f, 0.0f, 1.0f, 0.0f);
	glUniformMatrix4fv(m_loc_MVP, 1, GL_FALSE, &( MVP[0][0] )); // Send to the VGA

	// Draw top cap
	glBindVertexArray(m_VAO_ID[1]);
	glDrawElements(GL_TRIANGLES, 3 * 2 * N, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	// Rotate by 180° and translate by 'height'
	MVP = glm::translate<float>(MVP, height, 0.0f, 0.0f);
	MVP = glm::rotate(MVP, 180.0f, 0.0f, 1.0f, 0.0f);
	glUniformMatrix4fv(m_loc_MVP, 1, GL_FALSE, &( MVP[0][0] )); // Send to the VGA

	// Draw bottom cap
	glBindVertexArray(m_VAO_ID[1]);
	glDrawElements(GL_TRIANGLES, 3 * 2 * N, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	// Discard changes (reverse order)
	MVP = glm::rotate(MVP, -180.0f, 0.0f, 1.0f, 0.0f);
	MVP = glm::translate<float>(MVP, -height, 0.0f, 0.0f);
	MVP = glm::translate(MVP, -xyz);
	glUniformMatrix4fv(m_loc_MVP, 1, GL_FALSE, &( MVP[0][0] )); // Send to the VGA
}

void Application::Update() {
	static Uint32 t = SDL_GetTicks();
	m_cam.Update((SDL_GetTicks() - t)/1000.0f);
	t = SDL_GetTicks();

	// Calculate scaling iff we need to
	if(scale) m_scaling = powf(2.0f, sinf(((t % 10000) / 10000.0f) * 2.0f * M_PI));
	// Calculate rotation angle
	m_rotation = ( ( t % 11000 ) / 11000.0f )*2.0f*M_PI;
}

void Application::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Set currently used shader program
	glUseProgram(m_program_ID);

	// Set world matrix
	m_world = glm::mat4(1.0f);

	// Set initial Model-View-Projection mx.
	glm::mat4 MVP = m_cam.getViewProj() * m_world;

	float gy8 = sqrtf(8.0f);
	float gy12 = sqrtf(12.0f);
	float down = gy12 - gy8;
	// ====================================================================================================
	// ============================== Moving along circular path ==========================================
	// = According to http://gamedev.stackexchange.com/questions/9607/moving-an-object-in-a-circular-path =
	glm::vec3 rotation(0.0f + sinf(m_rotation)*10.0f, 0.0f + cosf(m_rotation)*10.0f, 0.0f);
	MVP = glm::translate(MVP, rotation);

	// ====================================================================================================
	// ============================== Scaling on Z-axis ===================================================
	MVP = glm::scale(MVP, 1.0f, 1.0f, m_scaling);

	// ====================================================================================================
	// ============================== Rendering the 3 cylinders ===========================================
	RenderCylinder(MVP, glm::vec3(0.0f, gy8, 0.0f));
	RenderCylinder(MVP, glm::vec3(gy8, down, 0.0f));
	RenderCylinder(MVP, glm::vec3(-down, down, 2.0f));
	RenderCylinder(MVP, glm::vec3(-down, down, -2.0f));

	// ====================================================================================================
	// ============================== Undoing the translation =============================================
	MVP = glm::translate(MVP, -rotation);
	glUniformMatrix4fv(m_loc_MVP, 1, GL_FALSE, &( MVP[0][0] )); // Send to the VGA

	// Draw axes
	if( draw_z ) {
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 1000.0f);
		glVertex3f(0.0f, 0.0f, -1000.0f);
		glEnd();
	}
	if( draw_x ) {
		glBegin(GL_LINES);
		glVertex3f(1000.0f, 0.0f, 0.0f);
		glVertex3f(-1000.0f, 0.0f, 0.0f);
		glEnd();
	}
	if( draw_y ) {
		glBegin(GL_LINES);
		glVertex3f(0.0f, 1000.0f, 0.0f);
		glVertex3f(0.0f, -1000.0f, 0.0f);
		glEnd();
	}
	
	// We're not using the shader program anymore
	glUseProgram(0);
}

void Application::Clear() {
	glDeleteBuffers(2, m_IB_ID);
	glDeleteBuffers(2, m_VBO_ID);
	glDeleteVertexArrays(2, m_VAO_ID);

	glDeleteProgram(m_program_ID);
}

void Application::KeyDown(const SDL_KeyboardEvent& key) {
	switch( key.keysym.sym ) {
	case SDLK_x:
		draw_x = !draw_x;
		break;
	case SDLK_y:
		draw_y = !draw_y;
		break;
	case SDLK_z:
		draw_z = !draw_z;
		break;
	case SDLK_SPACE:
		scale = !scale;
		break;
	default:
		break;
	}
	m_cam.KeyEvent(key, true);
}

void Application::KeyUp(const SDL_KeyboardEvent& key) {
	m_cam.KeyEvent(key, false);
}

void Application::MouseMove(const SDL_MouseMotionEvent& mouse) {
	m_cam.MouseMoveEvent(mouse);
}

void Application::Resize(Sint32 w, Sint32 h) {
	glViewport(0, 0, w, h);
	//m_projection = glm::perspective(45.0f, w / static_cast<float>( h ), 1.0f, 1000.0f);
	m_cam.resize(w, h);
}
