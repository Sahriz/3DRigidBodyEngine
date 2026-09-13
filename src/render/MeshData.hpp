#pragma once

#include <glm/vec3.hpp>
#include <vector>

struct MeshData {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
};
