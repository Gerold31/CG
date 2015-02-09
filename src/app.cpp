#include "app.h"

#include <cmath>
#include <cstdlib>
#include <memory>
#include <mutex>

#include <GL/glew.h>

#include <SDL2/SDL.h>

#include "camera.h"
#include "chessboard.h"
#include "light.h"
#include "logging.h"
#include "meshinstance.h"
#include "ocean.h"
#include "onscreentext.h"
#include "scene.h"
#include "skybox.h"
#include "types.h"

std::unique_ptr<App> App::mInstance;
std::once_flag App::mInstanceFlag;

int App::run()
{
	mRunning = true;
	// initialize
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		SEVERE("SDL initialization failed: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

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

	SDL_SetRelativeMouseMode(SDL_TRUE);

	glewExperimental = GL_TRUE;
	GLenum ret = glewInit();
	if (ret != GLEW_OK) {
		SEVERE("Glew initialization failed: %s", glewGetErrorString(ret));
		return EXIT_FAILURE;
	}

	INFO("Create file system.");
	mFS.reset(new FileSystem("."));

	INFO("Create scene.");
	setupScene();

	// set some render options
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// run main loop
	Uint32 currentTime = SDL_GetTicks();
	Uint32 fps_counter = 0;
	Uint32 fps_lastSample = currentTime;
	Uint32 lastUpdateTime = currentTime;
	while (mRunning)
	{
		// update fps
		currentTime = SDL_GetTicks();
		Uint32 fps_diff = currentTime - fps_lastSample;
		if (fps_diff > 1000) {
			Uint32 fps = fps_counter * 1000 / fps_diff;
			static char buf[16];
			std::sprintf(buf, "FPS: %d", fps);
			mFpsText->setText(buf);
			// reset counter
			fps_lastSample = currentTime;
			fps_counter = 0;
		}
		++fps_counter;

		// poll events
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				mRunning = false;
			else if (e.type == SDL_KEYDOWN)
				handleKeyDownEvent(e.key);
			else if (e.type == SDL_KEYUP)
				handleKeyUpEvent(e.key);
			else if (e.type == SDL_MOUSEMOTION)
				handleMouseMotion(e.motion);
		}

		// update scene
		currentTime = SDL_GetTicks();
		float elapsedTime = (currentTime - lastUpdateTime) * 0.001f;
		lastUpdateTime = currentTime;

		update(elapsedTime);
		mScene->update(elapsedTime);

		// draw scene
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		mScene->draw();

		// wait
		//Uint32 remainingTime = 17 - (SDL_GetTicks() - startTime);
		//if (remainingTime > 0) {
		//	SDL_Delay(remainingTime);
		//}

		// show on display
		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}

void App::setupScene()
{
	mScene.reset(new Scene());
	float aspectRatio = (float) mResolution.x / mResolution.y;

	// scene setting
	mScene->setAmbientLight(Color(0.1f, 0.1f, 0.1f));

	// setup cam
	mCam = std::make_shared<Camera>(M_PI_4, 0.1f, 1000.f, aspectRatio);
	mCam->setPosition(Vec3(10.f, 5.f, 10.f));
	mCam->lookAt(Vec3(0.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f));
	mScene->setCamera(mCam);

	// create objects
	mFpsText = std::make_shared<OnScreenText>("FPS: -");
	mFpsText->setPosition(Vec3(-0.9f, -0.9f, 0.f));

	std::shared_ptr<SkyBox> skyBox = std::make_shared<SkyBox>();

	std::shared_ptr<ChessBoard> chessBoard = std::make_shared<ChessBoard>();

	std::shared_ptr<Ocean> ocean = std::make_shared<Ocean>();
	ocean->setPosition(Vec3(0.f, -0.3f, 0.f));

	std::shared_ptr<Mesh> pawn = std::make_shared<Mesh>("chesspiece_pawn");
	std::shared_ptr<Mesh> rook = std::make_shared<Mesh>("chesspiece_rook");
	std::shared_ptr<Mesh> knight = std::make_shared<Mesh>("chesspiece_knight");
	std::shared_ptr<Mesh> bishop = std::make_shared<Mesh>("chesspiece_bishop");
	std::shared_ptr<Mesh> queen = std::make_shared<Mesh>("chesspiece_queen");
	std::shared_ptr<Mesh> king = std::make_shared<Mesh>("chesspiece_king");


	std::shared_ptr<MeshInstance> pawns[2][8];
	std::shared_ptr<MeshInstance> rooks[2][2];
	std::shared_ptr<MeshInstance> knights[2][2];
	std::shared_ptr<MeshInstance> bishops[2][2];
	std::shared_ptr<MeshInstance> queens[2][1];
	std::shared_ptr<MeshInstance> kings[2][1];


	// add drawable objects to scene
	mScene->add(skyBox);
	mScene->add(ocean);
	mScene->add(chessBoard);
	mScene->add(mFpsText);

	chessBoard->setPosition(Vec3(1, 0.9, 1));

	for(int j=0; j<2; j++)
	{
		for(int i=0; i<8; i++)
		{
			pawns[j][i] = std::make_shared<MeshInstance>(pawn, j);
			pawns[j][i]->setPosition(Vec3(j ? -5 : 5, 1, -7 + 2*i));
			mScene->add(pawns[j][i]);
		}
		for(int i=0; i<2; i++)
		{
			rooks[j][i] = std::make_shared<MeshInstance>(rook, j);
			rooks[j][i]->setPosition(Vec3(j ? -7 : 7, 1, i ? -7 : 7));
			mScene->add(rooks[j][i]);
			knights[j][i] = std::make_shared<MeshInstance>(knight, j);
			knights[j][i]->setPosition(Vec3(j ? -7 : 7, 1, i ? -5 : 5));
			knights[j][i]->rotate(j ? M_PI/2 : -M_PI/2, Vec3(0, 1, 0));
			mScene->add(knights[j][i]);
			bishops[j][i] = std::make_shared<MeshInstance>(bishop, j);
			bishops[j][i]->setPosition(Vec3(j ? -7 : 7, 1, i ? -3 : 3));
			mScene->add(bishops[j][i]);
		}
		queens[j][0] = std::make_shared<MeshInstance>(queen, j);
		queens[j][0]->setPosition(Vec3(j ? -7 : 7, 1, -1));
		mScene->add(queens[j][0]);
		kings[j][0] = std::make_shared<MeshInstance>(king, j);
		kings[j][0]->setPosition(Vec3(j ? -7 : 7, 1, 1));
		mScene->add(kings[j][0]);
	}


	// setup light
	std::shared_ptr<Light> sun = std::make_shared<Light>();
	sun->setColor(Vec3(0.6f, 0.6f, 0.5f));
	sun->setPosition(Vec4(1000.f, 1000.f, 1000.f, 1.f));
	sun->setAttenuation(0.f);
	mScene->add(sun);

	std::shared_ptr<Light> light1 = std::make_shared<Light>();
	light1->setColor(Vec3(1.f, 1.f, 1.f));
	light1->setPosition(Vec4(0.f, 5.f, 0.f, 1.f));
	light1->setAttenuation(0.1f);
	mScene->add(light1);

	std::shared_ptr<Light> light2 = std::make_shared<Light>();
	light2->setColor(Vec3(1.f, .5f, .2f));
	light2->setPosition(Vec4(8.f, 3.f, -8.f, 1.f));
	light2->setAttenuation(0.01f);
	mScene->add(light2);
}

void App::destroyScene()
{
	mCam = nullptr;
	mFpsText = nullptr;
	mScene = nullptr;
}

void App::update(float elapsedTime)
{
	float factor = mMovementSpeed * elapsedTime;
	mCam->move(Vec3(factor * mXMovement, factor * mYMovement,
					factor * mZMovement));
}

void App::handleKeyDownEvent(SDL_KeyboardEvent &e)
{
	SDL_Keycode keyPressed = e.keysym.sym;
	if (e.repeat)
		return;

	switch (keyPressed)
	{
	case SDLK_w:
		mZMovement -= 1.f;
		break;
	case SDLK_a:
		mXMovement -= 1.f;
		break;
	case SDLK_s:
		mZMovement += 1.f;
		break;
	case SDLK_d:
		mXMovement += 1.f;
		break;
	case SDLK_SPACE:
		mYMovement += 1.f;
		break;
	case SDLK_LCTRL:
		mYMovement -= 1.f;
		break;
	}
}

void App::handleKeyUpEvent(SDL_KeyboardEvent &e)
{
	SDL_Keycode keyPressed = e.keysym.sym;
	if (e.repeat)
		return;

	switch (keyPressed)
	{
	case SDLK_ESCAPE:
		mRunning = false;
		break;
	case SDLK_w:
		if (mZMovement < 0.f)
			mZMovement = 0.f;
		break;
	case SDLK_a:
		if (mXMovement < 0.f)
			mXMovement = 0.f;
		break;
	case SDLK_s:
		if (mZMovement > 0.f)
			mZMovement = 0.f;
		break;
	case SDLK_d:
		if (mXMovement > 0.f)
			mXMovement = 0.f;
		break;
	case SDLK_SPACE:
		if (mYMovement > 0.f)
			mYMovement = 0.f;
		break;
	case SDLK_LCTRL:
		if (mYMovement < 0.f)
			mYMovement = 0.f;
		break;
	}
}

void App::handleMouseMotion(SDL_MouseMotionEvent &e)
{
	// show up or down
	mCam->rotate(mRotationSpeed * e.yrel, Vec3(-1.f, 0.f, 0.f));
	// show left or right
	Vec3 v = (Mat3) mCam->getTransfToLocale() * Vec3(0.f, -1.f, 0.f);
	mCam->rotate(mRotationSpeed * e.xrel, v);
}
