#ifndef LIGHT_H
#define LIGHT_H

#include "object.h"

class Light : public Object
{
public:
	Light();
    ~Light();

	void update(float elapsedTime) {}

	void setColor(Vec3 color) { mColor = color; }
	void setAttenuation(float attenuation) { mAttenuation = attenuation; }
	Vec3 getColor() { return mColor; }
	float getAttenuation() { return mAttenuation; }

private:
	Vec3 mColor;
	float mAttenuation;
};

#endif // LIGHT_H
