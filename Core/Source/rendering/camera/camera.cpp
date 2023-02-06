#include "pch.h"

#include "rendering/camera/camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace rendering
{
    camera::camera(const camera_specification& specification)
    {
        m_camera_specification = specification;
        setup_matrices();
    }

    camera::~camera()
    {
    }

    void camera::setup_matrices()
    {
        m_projection = glm::perspective(glm::radians(m_camera_specification.fov), m_camera_specification.aspectRatio,
            m_camera_specification.nearPlane, m_camera_specification.farPlane);
        m_view = glm::lookAt(m_camera_specification.position, m_camera_specification.position + m_front, m_up);
    }

    const glm::mat4& camera::get_view_projection() const
    {
        return m_projection * m_view;
    }

    const glm::mat4& camera::get_view() const
    {
        return m_view;
    }

    const glm::mat4& camera::get_projection() const
    {
        return m_projection;
    }
}
