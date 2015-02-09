#ifndef APP_H
#define APP_H

#include <memory>
#include <mutex>

#include <SDL2/SDL.h>

#include "camera.h"
#include "filesystem.h"
#include "onscreentext.h"
#include "scene.h"
#include "types.h"


#define APP App::getInstance()

class App
{
public:
	FileSystem *getFileSystem();

	const Vec2i &getResolution() const;

	int run();

private:
	void setupScene();
	void destroyScene();
	void update(float elapsedTime);
	void handleKeyDownEvent(SDL_KeyboardEvent &e);
	void handleKeyUpEvent(SDL_KeyboardEvent &e);
	void handleMouseMotion(SDL_MouseMotionEvent &e);

	Vec2i mResolution;
	std::unique_ptr<FileSystem> mFS;

	bool mRunning;
	std::unique_ptr<Scene> mScene;
	std::shared_ptr<Camera> mCam;
	std::shared_ptr<OnScreenText> mFpsText;

	float mXMovement;
	float mYMovement;
	float mZMovement;

	float mMovementSpeed;
	float mRotationSpeed;

	// ------------------------------------------------------------------------
	// Member to make it a singelton.
	// ------------------------------------------------------------------------
public:
	static App *getInstance()
	{
		std::call_once(mInstanceFlag, createInstance);
		return mInstance.get();
	}
private:
	App() :
		mResolution(800, 600),
		mXMovement(0.f),
		mYMovement(0.f),
		mZMovement(0.f),
		mMovementSpeed(2.f),
		mRotationSpeed(0.0002f)
	{}
	App(const App &); // = delete;
	App &operator=(const App &); // = delete;
	static std::unique_ptr<App> mInstance;
	static std::once_flag mInstanceFlag;
	static void createInstance()
	{
		mInstance.reset(new App());
	}
};


inline const Vec2i &App::getResolution() const
{
	return mResolution;
}

inline FileSystem *App::getFileSystem()
{
	return mFS.get();
}

#endif // APP_H
