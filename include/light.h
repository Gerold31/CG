#ifndef LIGHT_H
#define LIGHT_H

#include "object.h"

class Light : public Object
{
public:
	Light();
    ~Light();

	void update(float elapsedTime) {}

	void setColor(Color color) { mColor = color; }
	void setAttenuation(float attenuation) { mAttenuation = attenuation; }
	Color getColor() { return mColor; }
	float getAttenuation() { return mAttenuation; }

private:
	Color mColor;
	float mAttenuation;
};

#endif // LIGHT_H
