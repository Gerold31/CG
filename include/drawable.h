#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "object.h"

class Camera;

class Drawable : public Object
{
public:
    Drawable();
    virtual ~Drawable();

    virtual void draw(const Camera &camera) const = 0;
};

#endif // DRAWABLE_H
