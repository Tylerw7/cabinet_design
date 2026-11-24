#include "Grid.h"
#include <glm/glm.hpp>

Grid::Grid(int halfExtent, float spacing)
{
    std::vector<float> vertices;
    std::vector<std::uint32_t> indices;
    std::uint32_t index = 0;

    float limit = halfExtent * spacing;

    auto addLine = [&](glm::vec3 a, glm::vec3 b)
    {
        vertices.insert(vertices.end(), { a.x, a.y, a.z, 1.0f, 1.0f, 1.0f });
        vertices.insert(vertices.end(), { b.x, b.y, b.z, 1.0f, 1.0f, 1.0f });
        indices.push_back(index++);
        indices.push_back(index++);
    };

    for (int i = -halfExtent; i <= halfExtent; ++i)
    {
        float pos = i * spacing;
        // Lines running along Z (varying X)
        addLine(glm::vec3(pos, 0.0f, -limit), glm::vec3(pos, 0.0f, limit));
        // Lines running along X (varying Z)
        addLine(glm::vec3(-limit, 0.0f, pos), glm::vec3(limit, 0.0f, pos));
    }

    m_mesh = std::make_unique<Mesh>(vertices, indices);
}