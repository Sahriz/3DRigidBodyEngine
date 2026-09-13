#include "Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 modelMatrix(const Transform& transform) {
    glm::mat4 translation = glm::translate(
        glm::mat4(1.0f),
        transform.position
    );

    glm::mat4 rotation = glm::mat4_cast(transform.rotation);

    glm::mat4 scale = glm::scale(
        glm::mat4(1.0f),
        transform.scale
    );

    return translation * rotation * scale;
}
