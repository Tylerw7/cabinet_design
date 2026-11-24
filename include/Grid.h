#ifndef GRID_H
#define GRID_H

#include "Mesh.h"
#include <vector>
#include <memory>

// Builds a flat grid on the XZ plane (Y = 0), centered at the origin.
// halfExtent: how many units the grid reaches in each direction.
// spacing: distance between lines, in world units.
class Grid
{
public:
    Grid(int halfExtent, float spacing);

    void draw() const { m_mesh->draw(); }

private:
    std::unique_ptr<Mesh> m_mesh;
};

#endif