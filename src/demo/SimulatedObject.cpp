#include "SimulatedObject.hpp"

SimulatedObject::SimulatedObject(MeshID mID, rbe::BodyID bID, Transform initialTransform)
    : mesh{mID}, body{}, transform{initialTransform}
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
