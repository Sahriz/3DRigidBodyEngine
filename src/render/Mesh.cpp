#include "Mesh.hpp"

#include <utility>

Mesh::Mesh(Mesh&& other) noexcept
    : vao(std::exchange(other.vao, 0)),
      positionVBO(std::exchange(other.positionVBO, 0)),
      normalVBO(std::exchange(other.normalVBO, 0)),
      ebo(std::exchange(other.ebo, 0)),
      indexCount(std::exchange(other.indexCount, 0))
{
}

void Mesh::init(const MeshData& data)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    positionVBO = createAttribBuffer(0, data.positions);
    normalVBO = createAttribBuffer(1, data.normals);

    ebo = createIndexBuffer(data.indices);

    indexCount = static_cast<GLsizei>(data.indices.size());
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &positionVBO);
    glDeleteBuffers(1, &normalVBO);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void Mesh::draw() const
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

GLuint Mesh::createAttribBuffer(GLuint location, const std::vector<glm::vec3>& data)
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(data.size() * sizeof(glm::vec3)),
        data.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        location,
        3,
        GL_FLOAT,
        GL_FALSE,
        static_cast<GLsizei>(sizeof(glm::vec3)),
        nullptr
    );

    glEnableVertexAttribArray(location);

    return buffer;
}

GLuint Mesh::createIndexBuffer(const std::vector<unsigned int>& indices)
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
        indices.data(),
        GL_STATIC_DRAW
    );

    return buffer;
}
