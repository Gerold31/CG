#include "scene.h"

#include <cassert>
#include <memory>

#include "camera.h"
#include "drawable.h"
#include "light.h"
#include "types.h"


Scene::Scene()
{

}

Scene::~Scene()
{
	for (shared_ptr<Drawable> &drawable : mObjects)
	{
		drawable->mScene = nullptr;
	}
	for (shared_ptr<Light> light : mLights)
	{
		light->mScene = nullptr;
	}
}

void Scene::draw() const
{
	if (mSkybox != nullptr)
	{
		// TODO render skybox
	}
	for (const shared_ptr<const Drawable> &drawable : mObjects)
	{
		drawable->draw(*mCamera);
	}
}

void Scene::update(float elapsedTime)
{
	mCamera->update(elapsedTime);
	for (shared_ptr<Drawable> &drawable : mObjects)
	{
		drawable->update(elapsedTime);
	}
	for (shared_ptr<Light> &light : mLights)
	{
		light->update(elapsedTime);
	}
}

void Scene::add(shared_ptr<Drawable> obj)
{
	assert(obj->mScene == nullptr);
	obj->mScene = this;
	mObjects.push_back(obj);
}

void Scene::add(shared_ptr<Light> light)
{
	assert(light->mScene == nullptr);
	light->mScene = this;
	mLights.push_back(light);
}

void Scene::setCamera(shared_ptr<Camera> camera)
{
	mCamera = camera;
}
