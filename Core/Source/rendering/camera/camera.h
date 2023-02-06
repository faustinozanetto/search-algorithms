#pragma once

#include <glm/glm.hpp>

namespace rendering
{
    struct camera_specification
    {
        glm::vec3 position;

        float aspectRatio;
        float fov;
        float nearPlane;
        float farPlane;
    };

    class camera
    {
    public:
        camera(const camera_specification& specification);
        ~camera();

        void setup_matrices();

        const glm::mat4& get_view_projection() const;
        const glm::mat4& get_view() const;
        const glm::mat4& get_projection() const;

    private:
        camera_specification m_camera_specification{};

        glm::vec3 m_front = glm::vec3(0, 0, -1);
        glm::vec3 m_up = glm::vec3(0, 1, 0);

        glm::mat4 m_projection;
        glm::mat4 m_view;
    };
}
