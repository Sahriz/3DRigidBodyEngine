#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
public:
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(int Width, int Height) const;
    void moveLocal(glm::vec3 direction, float deltaTime);
    void look(float yawDelta, float pitchDelta);

private:
    glm::vec3 getForward() const;

    glm::vec3 position{0.0f, 0.0f, 3.0f};
    float yaw = -90.0f;
    float pitch = 0.0f;
    float movementSpeed = 3.0f;
    float fieldOfView = 45.0f;
};
