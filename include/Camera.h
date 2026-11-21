#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>


class Camera
{
    public:
        Camera() = default;

        // The view matrix for the current orbit position.
        glm::mat4 getViewMatrix() const;

        // Where the camera actually sits in world space.
        glm::vec3 getPosition() const;

        // Orbit controls — called by input handling in stage 2.
        void orbit(float deltaYaw, float deltaPitch);
        void zoom(float deltaDistance);
        void pan(float deltaX, float deltaY);

    private:
        glm::vec3 m_target = glm::vec3(0.0f, 0.0f, 0.0f);

        float m_yaw      = -90.0f;  // degrees
        float m_pitch    = 20.0f;   // degrees, looking slightly down
        float m_distance = 5.0f;    // units from target

        // Limits
        float m_minPitch   = -89.0f;
        float m_maxPitch   =  89.0f;
        float m_minDistance = 0.5f;
        float m_maxDistance = 100.0f;

};

#endif