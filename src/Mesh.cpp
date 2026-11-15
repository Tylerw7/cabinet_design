#include "Mesh.h"

Mesh::Mesh(const std::vector<float>& vertices,
           const std::vector<std::uint32_t>& indices)
    : m_indexCount(static_cast<GLsizei>(indices.size()))
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(std::uint32_t),
                 indices.data(),
                 GL_STATIC_DRAW);

    // Layout: stride is 6 floats. Position at offset 0, color at offset 3.
    const GLsizei stride = 6 * sizeof(float);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    release();
}

void Mesh::release()
{
    // Buffers delete safely even if 0, but VAO guard keeps intent clear.
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    m_vao = m_vbo = m_ebo = 0;
}

Mesh::Mesh(Mesh&& other) noexcept
    : m_vao(other.m_vao), m_vbo(other.m_vbo),
      m_ebo(other.m_ebo), m_indexCount(other.m_indexCount)
{
    other.m_vao = other.m_vbo = other.m_ebo = 0;
    other.m_indexCount = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other)
    {
        release();
        m_vao = other.m_vao;
        m_vbo = other.m_vbo;
        m_ebo = other.m_ebo;
        m_indexCount = other.m_indexCount;
        other.m_vao = other.m_vbo = other.m_ebo = 0;
        other.m_indexCount = 0;
    }
    return *this;
}

void Mesh::draw() const
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}