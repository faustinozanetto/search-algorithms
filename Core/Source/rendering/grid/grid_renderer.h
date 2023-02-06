#pragma once

#include "glm/glm.hpp"

#include "rendering/shader/shader.h"

namespace rendering
{
    struct grid_tile
    {
        int color;

        grid_tile(): color(0)
        {
        }

        grid_tile(int color) : color(color)
        {
        }
    };

    struct grid_renderer_specification
    {
        glm::uvec2 grid_size;
        int cell_size;

        grid_renderer_specification() = default;

        grid_renderer_specification(const glm::uvec2& grid_size, int cell_size) : grid_size(grid_size),
            cell_size(cell_size)
        {
        }
    };

    class grid_renderer
    {
    public:
        grid_renderer(const grid_renderer_specification& specification);
        ~grid_renderer();

        void initialize();

        void draw();
        void populate();
        void update();

        void set_matrix(const glm::mat4& view_projection);

        void add_tile(glm::vec2 pos, const glm::vec3& color);
        void add_line(const glm::vec3& start, const glm::vec3& end);

        void randomize_tiles(int coverage);

        void set_all(const glm::vec3& color);
        void set_start_end(const glm::uvec2& start, const glm::uvec2& end);
        void update_start_end_tiles();
    private:
        void load_shaders();
        void setup_tiles_buffers();
        void setup_lines_buffers();

        void draw_tiles();
        void draw_lines();

        void clear_data();

        grid_renderer_specification m_grid_renderer_specification;

        shared<shader> m_lines_shader;
        shared<shader> m_quad_shader;

        unsigned int m_tiles_vao, m_tiles_vbo, m_tiles_ebo;
        unsigned int m_tiles_positions_buffer;
        unsigned int m_tiles_colors_buffer;
        std::vector<glm::vec3> m_colors;
        std::vector<glm::mat4> m_positions;

        unsigned int m_line_vao;
        unsigned int m_line_vbo;
        std::vector<float> m_line_vertices;

        glm::mat4 m_view_projection;

        glm::uvec2 m_start_location;
        glm::uvec2 m_end_location;

        glm::vec3 m_start_color;
        glm::vec3 m_end_color;
    };
}
