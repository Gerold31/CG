#ifndef OBJECT_H
#define OBJECT_H

#include "types.h"

class Object
{
public:
    Object();
    virtual ~Object();

    virtual void update(float elapsedTime) = 0;

private:
    Mat4 mTransformation;
};

#endif // OBJECT_H
