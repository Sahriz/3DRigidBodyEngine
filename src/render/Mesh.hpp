#pragma once

#include "MeshData.hpp"

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <vector>

class Mesh {
public:
    Mesh() = default;
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&&) = delete;

    void init(const MeshData& data);
    void draw() const;

private:
    GLuint vao = 0;
    GLuint positionVBO = 0;
    GLuint normalVBO = 0;
    GLuint ebo = 0;
    GLsizei indexCount = 0;

    GLuint createAttribBuffer(GLuint location, const std::vector<glm::vec3>& data);
    GLuint createIndexBuffer(const std::vector<unsigned int>& indices);
};
