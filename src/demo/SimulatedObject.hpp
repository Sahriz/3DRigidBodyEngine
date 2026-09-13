#pragma once

#include "core/Transform.hpp"
#include "render/MeshID.hpp"

class SimulatedObject {
public:
    SimulatedObject(MeshID id, Transform initialTransform);

    MeshID getMesh() const;
    Transform getTransform() const;

private:
    MeshID mesh;
    Transform transform;
    glm::vec3 visualScale{1.0f};
};
