#include "pch.h"

#include "rendering/grid/grid_renderer.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>
#include <stdlib.h>

#include "rendering/renderer/renderer.h"

namespace rendering
{
	grid_renderer::grid_renderer(const grid_renderer_specification& specification)
	{
		m_grid_renderer_specification = specification;
		m_start_color = { 0.0f, 1.0f, 0.0 };
		m_end_color = { 1.0f, 0.0f, 0.0f };
		load_shaders();
	}

	grid_renderer::~grid_renderer()
	{
	}

	void grid_renderer::initialize()
	{
		const int total_tiles = m_grid_renderer_specification.grid_size.x * m_grid_renderer_specification.grid_size.y;
		m_colors.reserve(total_tiles);
		m_positions.reserve(total_tiles);

		setup_tiles_buffers();
		setup_lines_buffers();
	}

	void grid_renderer::setup_lines_buffers()
	{
		std::vector<float> vertices = {
		};
		m_line_vertices.insert(m_line_vertices.end(), vertices.begin(), vertices.end());
		/* Generate lines vao and vbo. */
		glGenVertexArrays(1, &m_line_vao);
		glGenBuffers(1, &m_line_vbo);
		glBindVertexArray(m_line_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_line_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_line_vertices.size(), m_line_vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void grid_renderer::draw()
	{
		draw_tiles();
		draw_lines();
	}

	void grid_renderer::populate()
	{
		for (int i = 0; i < m_grid_renderer_specification.grid_size.x; i++)
		{
			for (int j = 0; j < m_grid_renderer_specification.grid_size.y; j++)
			{
				add_tile(glm::vec2(i, j), glm::vec3(0));
			}
		}

		// draw horizontal lines of grid
		for (int j = 0; j <= m_grid_renderer_specification.grid_size.y; j++)
		{
			add_line(glm::vec3(0, j, 0), glm::vec3(m_grid_renderer_specification.grid_size.x, j, 0));
		}
		// draw vertical lines of grid
		for (int i = 0; i <= m_grid_renderer_specification.grid_size.x; i++)
		{
			add_line(glm::vec3(i, 0, 0), glm::vec3(i, m_grid_renderer_specification.grid_size.y, 0));
		}
	}

	void grid_renderer::update()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_tiles_positions_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_positions.size() * sizeof(glm::mat4),
			&m_positions.front());

		glBindBuffer(GL_ARRAY_BUFFER, m_tiles_colors_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_colors.size() * sizeof(glm::vec3), &m_colors.front());
	}

	void grid_renderer::set_matrix(const glm::mat4& view_projection)
	{
		m_view_projection = view_projection;
	}

	void grid_renderer::add_tile(glm::vec2 pos, const glm::vec3& color)
	{
		const glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(pos, 0.0));
		m_positions.push_back(model);
		m_colors.push_back(color);
	}

	void grid_renderer::add_line(const glm::vec3& start, const glm::vec3& end)
	{
		std::vector vertices = {
			start.x, start.y, start.z,
			end.x, end.y, end.z,
		};

		m_line_vertices.insert(m_line_vertices.end(), vertices.begin(), vertices.end());
		// Update line VBO and buffer.
		glBindVertexArray(m_line_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_line_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_line_vertices.size(), m_line_vertices.data(), GL_STATIC_DRAW);
	}

	void grid_renderer::randomize_tiles(int coverage)
	{
		//        set_all(glm::vec3(1.0f));
		clear_data();
		for (int i = 0; i < m_grid_renderer_specification.grid_size.x; i++)
		{
			for (int j = 0; j < m_grid_renderer_specification.grid_size.y; j++)
			{
				glm::uvec2 position = { i, j };
				glm::vec3 color = glm::vec3{ 0.85f };

				// Only covers a certain percentage of the tiles.
				if (rand() % 100 < coverage)
				{
					add_tile(position, color);
				}
				else
				{
					add_tile(position, glm::vec3(0.15f));
				}
			}
		}
		update_start_end_tiles();
		update();
	}

	void grid_renderer::set_all(const glm::vec3& color)
	{
		clear_data();
		for (int i = 0; i < m_grid_renderer_specification.grid_size.x; i++)
		{
			for (int j = 0; j < m_grid_renderer_specification.grid_size.y; j++)
			{
				add_tile(glm::vec2(i, j), color);
			}
		}
	}

	void grid_renderer::set_start_end(const glm::uvec2& start, const glm::uvec2& end)
	{
		m_start_location = start;
		m_end_location = end;
	}

	void grid_renderer::update_start_end_tiles()
	{
		int startIndex = m_start_location.y * m_grid_renderer_specification.grid_size.x + m_start_location.x;
		int endIndex = m_end_location.y * m_grid_renderer_specification.grid_size.x + m_end_location.x;
		m_colors.at(startIndex) = m_start_color;
		m_colors.at(endIndex) = m_end_color;
	}

	void grid_renderer::load_shaders()
	{
		m_lines_shader = shader::create({ "../Assets/Shaders/Line.vert", "../Assets/Shaders/Line.frag" });
		m_quad_shader = shader::create({ "../Assets/Shaders/Quad.vert", "../Assets/Shaders/Quad.frag" });
	}

	void grid_renderer::setup_tiles_buffers()
	{
		float vertices[] = {
			1.0f, 1.0f, 0.0f, // top right
			1.0f, 0.0f, 0.0f, // bottom right
			0.0f, 0.0f, 0.0f, // bottom left
			0.0f, 1.0f, 0.0f // top left
		};

		unsigned int indices[] = {
			0, 1, 3, // first Triangle
			1, 2, 3 // second Triangle
		};

		/* Generate buffers and vao. */
		glGenVertexArrays(1, &m_tiles_vao);
		glGenBuffers(1, &m_tiles_vbo);
		glGenBuffers(1, &m_tiles_ebo);
		glGenBuffers(1, &m_tiles_positions_buffer);
		glGenBuffers(1, &m_tiles_colors_buffer);

		glBindVertexArray(m_tiles_vao);

		/* Setup vbo and ebo. */
		glBindBuffer(GL_ARRAY_BUFFER, m_tiles_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_tiles_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/* Setting up Positions Buffer */
		glBindBuffer(GL_ARRAY_BUFFER, m_tiles_positions_buffer);
		glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(glm::mat4), &m_positions.front(),
			GL_STATIC_DRAW);

		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(4);

		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);

		/* Setting up Color Buffer */
		glBindBuffer(GL_ARRAY_BUFFER, m_tiles_colors_buffer);

		glBindBuffer(GL_ARRAY_BUFFER, m_tiles_colors_buffer);
		glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec3), &m_colors.front(),
			GL_STATIC_DRAW);

		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(5);
		glVertexAttribDivisor(5, 1);

		/* Cleanup */
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void grid_renderer::draw_tiles()
	{
		m_quad_shader->bind();
		m_quad_shader->set_mat4("mvp", m_view_projection);
		glBindVertexArray(m_tiles_vao);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_positions.size());
		m_quad_shader->un_bind();
	}

	void grid_renderer::draw_lines()
	{
		m_lines_shader->bind();
		m_lines_shader->set_mat4("mvp", m_view_projection);
		glBindVertexArray(m_line_vao);
		glDrawArrays(GL_LINES, 0, m_line_vertices.size() / 3);
		m_lines_shader->un_bind();
	}

	void grid_renderer::clear_data()
	{
		m_positions.clear();
		m_colors.clear();

		const int total_tiles = m_grid_renderer_specification.grid_size.x * m_grid_renderer_specification.grid_size.y;
		m_colors.shrink_to_fit();
		m_positions.shrink_to_fit();
		m_colors.reserve(total_tiles);
		m_positions.reserve(total_tiles);
	}
}
