#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

glm::mat4 Camera::getViewMatrix() const
{
    glm::vec3 forward = getForward();

    return glm::lookAt(
        position,
        position + forward,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

glm::mat4 Camera::getProjectionMatrix(int Width, int Height) const
{
    return glm::perspective(glm::radians(fieldOfView), static_cast<float>(Width) / Height, 0.1f, 100.0f);
}

void Camera::moveLocal(glm::vec3 direction, float deltaTime)
{
    glm::vec3 forward = getForward();
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
    glm::vec3 up = glm::vec3(0, 1, 0);

    position += (
        right * direction.x +
        up * direction.y +
        forward * direction.z
        ) * movementSpeed * deltaTime;
}

void Camera::look(float yawDelta, float pitchDelta)
{
    yaw += yawDelta;
    pitch = std::clamp(pitch + pitchDelta, -89.0f, 89.0f);
}

glm::vec3 Camera::getForward() const
{
    const float yawRadiands = glm::radians(yaw);
    const float pitchRadians = glm::radians(pitch);

    glm::vec3 forward{
        std::cos(yawRadiands) * std::cos(pitchRadians),
        std::sin(pitchRadians),
        std::sin(yawRadiands) * std::cos(pitchRadians)
    };

    forward = glm::normalize(forward);
    return forward;
}
