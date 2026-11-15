#pragma once

#include <glad/gl.h>
#include <vector>
#include <cstdint>

class Mesh
{
public:
    // Interleaved vertex data: 3 floats position, 3 floats color per vertex.
    Mesh(const std::vector<float>& vertices,
         const std::vector<std::uint32_t>& indices);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void draw() const;

private:
    void release();

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
    GLsizei m_indexCount = 0;
};