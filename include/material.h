#ifndef MATERIAL_H
#define MATERIAL_H

#include <types.h>

class Texture;

class Material
{
public:
    Material(shared_ptr<Texture> diffuseMap, shared_ptr<Texture> specularMap, shared_ptr<Texture> bumpMap) :
        mDiffuseMap(diffuseMap), mSpecularMap(specularMap), mBumpMap(bumpMap) {}
    ~Material() {}

    shared_ptr<const Texture> getDiffuseMap() const { return mDiffuseMap; }
    shared_ptr<const Texture> getSpecularMap() const { return mSpecularMap; }
    shared_ptr<const Texture> getBumpMap() const { return mBumpMap; }

private:
    const shared_ptr<const Texture> mDiffuseMap;
    const shared_ptr<const Texture> mSpecularMap;
    const shared_ptr<const Texture> mBumpMap;
};

#endif // MATERIAL_H
