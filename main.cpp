#include <GL\glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <iostream>
#include <sstream>

#include "Application.hpp"

#define WAIT_FOR_VSYNC 1

void exitProgram();

int main(int argc, char* argv[]) {
	atexit( exitProgram );

	size_t width = 640, height = 480;

	if( SDL_Init(SDL_INIT_VIDEO) == -1 ) {
		std::wcout << L"[SDL ind�t�sa] Hiba az SDL inicializ�ci�j�ban: " << SDL_GetError() << '\n';
		return 1;
	}

	// �ll�tsuk be, hogy h�ny biten szeretn�nk t�rolni a piros, z�ld, k�k �s �tl�tszatlans�gi inform�ci�kat pixelenk�nt
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	// duplapufferel�s
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// m�lys�gi puffer h�ny bites legyen
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// 0
	// nullptr -> using nullptr = (void*)0;
	// NULL -> #define NULL 0
	SDL_Window* window = nullptr;
	window = SDL_CreateWindow(
		"Noszky vagyok", // Ablak 'neve'
		100,
		100,
		width,
		height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	if( !window ) {
		std::wcout << L"[Ablak l�trehoz�sa] Hiba az SDL inicializ�ci�j�ban: " << SDL_GetError() << '\n';
		return 2;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);
	if( context == nullptr ) {
		std::wcout << L"[OpenGL context l�trehoz�sa] Hiba az SDL inicializ�ci�j�ban: " << SDL_GetError() << '\n';
		return 3;
	}

	SDL_GL_SetSwapInterval(WAIT_FOR_VSYNC);

	GLenum error = glewInit();
	if( error != GLEW_OK ) {
		std::wcout << L"[GLEW] Hiba a GLEW inicializ�l�sa k�zben: " << error << '\n';
		return 4;
	}

	int gl_version[2] = { -1, - 1 };
	glGetIntegerv(GL_MAJOR_VERSION, gl_version);
	glGetIntegerv(GL_MINOR_VERSION, gl_version + 1);
	std::cout << "Haszn�lt OpenGL verzi�: " << gl_version[0] << '.' << gl_version[1] << '\n';

	if( gl_version[0] == -1 || gl_version[1] == -1 ) {
		std::wcout << L"[OpenGL context l�trehoz�sa] Nem siker�lt l�trehozni az OpenGL context-et." << '\n';
		return 5;
	}

	bool quit = false;

	Application app;
	if( !app.Init() ) {
		SDL_DestroyWindow(window);
		std::wcout << L"[app.Init] Az alkalmaz�s inicializ�l�sa k�zben hiba t�rt�nt!" << '\n';
		return 6;
	}

	SDL_Event ev;
	while( !quit ) {
		if( SDL_PollEvent(&ev) ) {
			switch( ev.type ) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if( ev.key.keysym.sym == SDLK_ESCAPE )
					quit = true;
				app.KeyDown(ev.key);
				break;
			case SDL_KEYUP:
				app.KeyUp(ev.key);
				break;
			case SDL_WINDOWEVENT:
				if( ev.window.event == SDL_WINDOWEVENT_RESIZED )
					width = ev.window.data1, height = ev.window.data2;
					app.Resize(width, height);
				break;
			case SDL_MOUSEMOTION:
				app.MouseMove(ev.motion);
				break;
			default:
				break;
			}
		}
		app.Update();
		app.Render();

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);

	return 0;
}

void exitProgram() {
	SDL_Quit();

	std::cout << "Nyomjon meg egy gombot...\n";
	std::cin.get();
}
