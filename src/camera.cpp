#include "camera.h"


Camera::Camera(float fov, float near, float far, float aspect) :
	mFov{fov},
	mNear{near},
	mFar{far},
	mAspect{aspect}
{
}

Camera::~Camera()
{

}

void Camera::update(float elapsedTime)
{
	// Nothing to do...
}
