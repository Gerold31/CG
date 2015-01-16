#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"

class Camera : public Object
{
public:
    Camera();
    ~Camera();

private:
    float mFov;
};

#endif // CAMERA_H
