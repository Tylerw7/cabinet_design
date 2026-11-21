#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>


glm::vec3 Camera::getPosition() const
{
    // Convert yaw/pitch/distance (spherical) into an x,y,z offset
    // from the target, then add the target.
    float yawRad   = glm::radians(m_yaw);
    float pitchRad = glm::radians(m_pitch);

    glm::vec3 offset;
    offset.x = m_distance * std::cos(pitchRad) * std::cos(yawRad);
    offset.y = m_distance * std::sin(pitchRad);
    offset.z = m_distance * std::cos(pitchRad) * std::sin(yawRad);

    return m_target + offset;
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(
        getPosition(),                 // eye
        m_target,                      // center we look at
        glm::vec3(0.0f, 1.0f, 0.0f)    // up direction (world Y)
    );
}

void Camera::orbit(float deltaYaw, float deltaPitch)
{
    m_yaw   += deltaYaw;
    m_pitch += deltaPitch;
    m_pitch  = std::clamp(m_pitch, m_minPitch, m_maxPitch);
}

void Camera::zoom(float deltaDistance)
{
    m_distance += deltaDistance;
    m_distance  = std::clamp(m_distance, m_minDistance, m_maxDistance);
}

void Camera::pan(float deltaX, float deltaY)
{
    // Move the target within the camera's own right/up plane,
    // so panning feels screen-relative.
    glm::vec3 forward = glm::normalize(m_target - getPosition());
    glm::vec3 right   = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
    glm::vec3 up      = glm::normalize(glm::cross(right, forward));

    m_target += right * deltaX + up * deltaY;
}