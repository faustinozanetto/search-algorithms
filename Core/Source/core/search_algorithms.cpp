#include "pch.h"

#include "search_algorithms.h"

#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "rendering/renderer/renderer.h"
#include "ui/ui.h"

namespace core
{
	search_algorithms* search_algorithms::s_instance = nullptr;

	search_algorithms::search_algorithms(const search_algorithms_specification& specification)
	{
		s_instance = this;
		m_specification = specification;
		m_state = application_state::start;
		m_algorithm = algorithm_type::a_star;

		initialize_window();
		initialize_camera();
		initialize_renderer();
		initialize_ui();
	}

	search_algorithms::~search_algorithms()
	{
	}

	void search_algorithms::run()
	{
		while (!rendering::renderer::should_close())
		{
			rendering::renderer::set_clear_color({ 0.0f, 0.0f, 0.0f, 1.0f });
			rendering::renderer::clear_screen();

			m_user_interface->being_frame_render();
			m_grid_renderer->draw();
			ImGui::Begin("Debug");
			if (ImGui::Button("Randomize"))
			{
				m_grid_renderer->randomize_tiles(70);
			}
			if (ImGui::DragFloat2("Start Location", glm::value_ptr(m_start_location), 1, 0, m_specification.grid_renderer_specification.grid_size.x - 1))
			{
				m_grid_renderer->set_start_end(m_start_location, m_end_location);
				m_grid_renderer->update_start_end_tiles();
				m_grid_renderer->update();
			}
			if (ImGui::DragFloat2("End Location", glm::value_ptr(m_end_location), 1, 0, m_specification.grid_renderer_specification.grid_size.y - 1))
			{
				m_grid_renderer->set_start_end(m_start_location, m_end_location);
				m_grid_renderer->update_start_end_tiles();
				m_grid_renderer->update();
			}
			ImGui::Text("Start: (%i, %i)", m_start_location.x, m_start_location.y);
			ImGui::Text("End: (%i, %i)", m_end_location.x, m_end_location.y);
			ImGui::ShowMetricsWindow();
			ImGui::End();
			m_user_interface->end_frame_render();

			rendering::renderer::swap_buffers();
			rendering::renderer::poll_input();
		}
	}

	search_algorithms& search_algorithms::get_application()
	{
		return *s_instance;
	}

	void search_algorithms::initialize_window()
	{
		m_window = rendering::window::create(m_specification.window_specification);
		if (!m_window->initialize_window())
			return;
		rendering::renderer::initialize({ m_window->get_native_window() });
	}

	void search_algorithms::initialize_camera()
	{
		const float aspectRatio = m_specification.window_specification.width / m_specification.window_specification.height;
		const rendering::camera_specification camera_specification = {
			m_camera_pos, aspectRatio, 90.0f, 0.1f, 1000.0f
		};
		m_camera = create_unique<rendering::camera>(camera_specification);
	}

	void search_algorithms::initialize_renderer()
	{
		m_grid_renderer = create_unique<rendering::grid_renderer>(m_specification.grid_renderer_specification);
		m_grid_renderer->set_matrix(m_camera->get_view_projection());
		m_start_location = { 0,0 };
		m_end_location = { m_specification.grid_renderer_specification.grid_size.x - 1, m_specification.grid_renderer_specification.grid_size.y - 1 };
		m_grid_renderer->set_start_end(m_start_location, m_end_location);
		m_grid_renderer->populate();
		m_grid_renderer->initialize();
		m_grid_renderer->update();
		m_grid_renderer->randomize_tiles(40);
	}

	void search_algorithms::initialize_ui()
	{
		m_user_interface = create_unique<ui::ui>();
		m_user_interface->initialize(m_window->get_native_window());
	}
}
