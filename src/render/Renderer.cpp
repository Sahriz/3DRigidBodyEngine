#include "Renderer.hpp"

#include <glm/gtc/type_ptr.hpp>

void Renderer::init()
{
    shader.InitProgram(SHADER_DIR);
}

MeshID Renderer::createMesh(const MeshData& data)
{
    MeshID id{ meshes.size() };

    meshes.emplace_back();
    meshes.back().init(data);

    return id;
}

void Renderer::beginFrame(const Camera camera, int framebufferWidth, int framebufferHeight)
{
    glClearColor(0.2, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = camera.getViewMatrix();
    projection = camera.getProjectionMatrix(framebufferWidth, framebufferHeight);
}

void Renderer::draw(MeshID id, const glm::mat4& model)
{
    GLint viewLoc = glGetUniformLocation(shader.getShader(), "view");
    GLint projectionLoc = glGetUniformLocation(shader.getShader(), "projection");
    GLint modelLoc = glGetUniformLocation(shader.getShader(), "model");

    glUseProgram(shader.getShader());
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    Mesh& mesh = meshes[id.index];
    mesh.draw();
}
