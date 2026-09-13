#include "SimulatedObject.hpp"

SimulatedObject::SimulatedObject(MeshID id, Transform initialTransform)
    : mesh{id}, transform{initialTransform}
{
}

MeshID SimulatedObject::getMesh() const
{
    return mesh;
}

Transform SimulatedObject::getTransform() const
{
    return transform;
}
