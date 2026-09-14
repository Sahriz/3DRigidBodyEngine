#include "SimulatedObject.hpp"

SimulatedObject::SimulatedObject(MeshID mID, rbe::BodyID bID, Transform initialTransform)
    : mesh{mID}, body{bID}, transform{initialTransform}
{
}

MeshID SimulatedObject::getMesh() const
{
    return mesh;
}

rbe::BodyID SimulatedObject::getBody() const
{
    return body;
}

Transform SimulatedObject::getTransform() const
{
    return transform;
}

void SimulatedObject::setTransform(const Transform& newTransform) {
	transform = newTransform;
}
