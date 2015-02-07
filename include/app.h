#ifndef APP_H
#define APP_H

#include <memory>
#include <mutex>

#include "filesystem.h"
#include "types.h"


#define APP App::getInstance()

class App
{
public:
	FileSystem *getFileSystem();

	int run();

private:
	Vec2i mResolution;
	std::unique_ptr<FileSystem> mFS;

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
		mResolution(800, 600)
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


inline FileSystem *App::getFileSystem()
{
	return mFS.get();
}

#endif // APP_H
