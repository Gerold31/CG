#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "object.h"
#include "types.h"


class Camera : public Object
{
public:
	Camera(float fov, float near, float far, float aspect);
    ~Camera();

	void setFov(float fov);
	void setNearClip(float near);
	void setFarClip(float far);
	void setAspect(float aspect);

	float getFov() const { return mFov; }
	float getNearClip() const { return mNear; }
	float getFarClip() const { return mFar; }
	float getAspect() const { return mAspect; }

	const Mat4 getProjection() const;

	virtual void update(float elapsedTime);

private:
    float mFov;
	float mNear;
	float mFar;
	float mAspect;

	mutable bool mNeedUpdate;
	mutable Mat4 mPerspective;
};


inline void Camera::setFov(float fov)
{
	if (mFov != fov) {
		mFov = fov;
		mNeedUpdate = true;
	}
}

inline void Camera::setNearClip(float near)
{
	if (mNear != near) {
		mNear = near;
		mNeedUpdate = true;
	}
}

inline void Camera::setFarClip(float far)
{
	if (mFar != far) {
		mFar = far;
		mNeedUpdate = true;
	}
}

inline void Camera::setAspect(float aspect)
{
	if (mAspect != aspect) {
		mAspect = aspect;
		mNeedUpdate = true;
	}
}

inline const Mat4 Camera::getProjection() const
{
	if (mNeedUpdate) {
		mNeedUpdate = false;
		mPerspective = glm::perspective(mFov, mAspect, mNear, mFar);
	}
	return mPerspective * getTransfToLocale();
}

#endif // CAMERA_H
