#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "types.h"

class Drawable;
class Light;
class Camera;
class Texture;

class Scene
{
public:
    Scene();
    ~Scene();

	void draw() const;
    void update(float elapsedTime);

    void add(shared_ptr<Drawable> obj);
    void add(shared_ptr<Light> light);
    void setCamera(shared_ptr<Camera> camera);
	void setAmbientLight(Color ambient) { mAmbientLight = ambient; }
    void setSkyBox(shared_ptr<Texture> skybox);

	size_t getNumLights() const { return mLights.size(); }
	shared_ptr<Light> getLight(size_t i) const { return mLights.at(i); }
	Color getAmbientLight() const { return mAmbientLight; }

private:
    std::vector<shared_ptr<Drawable> > mObjects;
    std::vector<shared_ptr<Light> > mLights;
    shared_ptr<Camera> mCamera;
    Color mAmbientLight;
    shared_ptr<Texture> mSkybox;
};

#endif // SCENE_H
