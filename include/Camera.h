#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>


class Camera
{
    public:
        Camera() = default;

        glm::mat4 getViewMatrix() const;
};

#endif