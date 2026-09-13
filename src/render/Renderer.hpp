#pragma once

#include "Camera.hpp"
#include "Mesh.hpp"
#include "MeshData.hpp"
#include "MeshID.hpp"
#include "Shader.hpp"

#include <glm/mat4x4.hpp>
#include <vector>

class Renderer {
public:
    void init();
    MeshID createMesh(const MeshData& data);
    void beginFrame(const Camera camera, int framebufferWidth, int framebufferHeight);
    void draw(MeshID id, const glm::mat4& model);

private:
    Shader shader;
    std::vector<Mesh> meshes;
    glm::mat4 view{1.0f};
    glm::mat4 projection{1.0f};
};
