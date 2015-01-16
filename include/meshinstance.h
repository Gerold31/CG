#ifndef MESHINSTANCE_H
#define MESHINSTANCE_H

#include "drawable.h"

class Mesh;
class Material;

class MeshInstance : public Drawable
{
public:
    MeshInstance();
    ~MeshInstance();

    virtual void draw(const Camera &camera) const;

private:
    shared_ptr<Mesh> mMesh;
    shared_ptr<Material> mMaterial;
};

#endif // MESHINSTANCE_H
