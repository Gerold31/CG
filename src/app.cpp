#include "app.h"

#include <cmath>
#include <memory>
#include <mutex>

#include <GL/glew.h>

#include <SDL2/SDL.h>

#include "camera.h"
#include "logging.h"
#include "scene.h"
#include "testbox.h"


std::unique_ptr<App> App::mInstance;
std::once_flag App::mInstanceFlag;

int App::run()
{
	// initialize
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_Window *window = SDL_CreateWindow(
				"Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				mResolution.x, mResolution.y, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	GLenum ret = glewInit();
	if (ret != GLEW_OK) {
		SEVERE("Glew initialization failed: %s", glewGetErrorString(ret));
	}

	INFO("Create file system.");
	mFS.reset(new FileSystem("."));

	INFO("Create scene.");
	Scene scene;
	float aspectRatio = (float) mResolution.x / mResolution.y;
	std::shared_ptr<Camera> cam = std::make_shared<Camera>(M_PI_4, 0.1f, 1000.f, aspectRatio);
	std::shared_ptr<TestBox> box = std::make_shared<TestBox>();

	scene.setCamera(cam);
	scene.add(box);

	cam->setPosition(Vec3(1.f, 3.f, 5.f));
	cam->lookAt(Vec3(0.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f));
	// TODO load scene

	// run main loop
	bool running = true;
	while (running)
	{
		Uint32 startTime = SDL_GetTicks();

		// poll events
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				running = false;
		}

		// update scene
		scene.update(1.f/60.f);

		// draw scene
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		scene.draw();

		// wait
		Uint32 remainingTime = 17 - (SDL_GetTicks() - startTime);
		if (remainingTime > 0) {
			SDL_Delay(remainingTime);
		}

		// show on display
		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);

	return EXIT_SUCCESS;
}
