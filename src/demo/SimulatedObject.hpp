#pragma once

#include "core/Transform.hpp"
#include "render/MeshID.hpp"
#include "RigidBodyEngine/BodyID.hpp"

class SimulatedObject {
public:
    SimulatedObject(MeshID mID, rbe::BodyID bID, Transform initialTransform);

    MeshID getMesh() const;
    Transform getTransform() const;

private:
    MeshID mesh;
    rbe::BodyID body;
    Transform transform;
    glm::vec3 visualScale{1.0f};
};
