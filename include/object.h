#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "types.h"

class Scene;


class Object
{
	friend class Scene;
public:
    Object();
    virtual ~Object();

    virtual void update(float elapsedTime) = 0;

	void setPosition(const Vec3 &pos);
	void setPosition(const Vec4 &pos);
	void move(const Vec3 &movement);
	void rotate(float angle, const Vec3 &v);
	void lookAt(const Vec3 &center, const Vec3 &up);

	const Mat4 &getTransfToLocale() const;
	const Mat4 &getTransfToGlobale() const;
	Vec4 getPosition() const;

protected:
	Scene *getScene();
	const Scene *getScene() const;

private:
	Scene *mScene;
	Mat4 mGlobaleToLocale;
	Mat4 mLocaleToGlobale;
};


inline void Object::setPosition(const Vec3 &pos)
{
	Vec3 lpos = Mat3(mGlobaleToLocale) * pos;
	mLocaleToGlobale[3] = Vec4(pos, 1.f);
	mGlobaleToLocale[3] = Vec4(-lpos, 1.f);
}

inline void Object::setPosition(const Vec4 &pos)
{
	Vec4 lpos(Mat3(mGlobaleToLocale) * Vec3(pos), pos.w);
	mLocaleToGlobale[3] = pos;
	mGlobaleToLocale[3] = -lpos;
}

inline void Object::move(const Vec3 &movement)
{
	Vec3 gmove = (Mat3) mLocaleToGlobale * movement;
	mLocaleToGlobale = glm::translate(mLocaleToGlobale, movement);
	mGlobaleToLocale = glm::translate(mGlobaleToLocale, -gmove);
}

inline void Object::rotate(float angle, const Vec3 &v)
{
	Mat4 rotationp = glm::rotate(Mat4(), angle, v);
	Mat4 rotationn = glm::rotate(Mat4(), -angle, v);
	mLocaleToGlobale = mLocaleToGlobale * rotationp;
	mGlobaleToLocale = rotationn * mGlobaleToLocale;
}

inline void Object::lookAt(const Vec3 &center, const Vec3 &up)
{
	Vec3 pos = (Vec3)getPosition();
	mGlobaleToLocale = glm::lookAt(pos, center, up);
	mLocaleToGlobale = glm::inverse(mGlobaleToLocale);
}

inline const Mat4 &Object::getTransfToLocale() const
{
	return mGlobaleToLocale;
}

inline const Mat4 &Object::getTransfToGlobale() const
{
	return mLocaleToGlobale;
}

inline Vec4 Object::getPosition() const
{
	return mLocaleToGlobale[3];
}

inline Scene *Object::getScene()
{
	return mScene;
}

inline const Scene *Object::getScene() const
{
	return mScene;
}

#endif // OBJECT_H
