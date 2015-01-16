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

    void update(float elapsedTime);

    void add(shared_ptr<Drawable> obj);
    void add(shared_ptr<Light> light);
    void setCamera(shared_ptr<Camera> camera);
    void setAmbientLight(Color ambient);
    void setSkyBox(shared_ptr<Texture> skybox);

private:
    std::vector<shared_ptr<Drawable> > mObjects;
    std::vector<shared_ptr<Light> > mLights;
    shared_ptr<Camera> mCamera;
    Color mAmbientLight;
    shared_ptr<Texture> mSkybox;
};

#endif // SCENE_H
