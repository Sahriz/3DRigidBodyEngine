#pragma once

#include "core/Transform.hpp"
#include "render/MeshID.hpp"
#include "RigidBodyEngine/BodyID.hpp"

class SimulatedObject {
public:
    SimulatedObject(MeshID mID, rbe::BodyID bID, Transform initialTransform);

    MeshID getMesh() const;
	rbe::BodyID getBody() const;
    Transform getTransform() const;

	void setTransform(const Transform& newTransform);

private:
    MeshID mesh;
    rbe::BodyID body;
    Transform transform;
    glm::vec3 visualScale{1.0f};
};
