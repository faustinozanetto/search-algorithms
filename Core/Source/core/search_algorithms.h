#pragma once

#include "rendering/camera/camera.h"
#include "rendering/grid/grid_renderer.h"
#include "rendering/window/window.h"
#include "ui/ui.h"

namespace core
{
	enum class application_state
	{
		start,
		customizing,
		simulating,
	};

	enum class algorithm_type
	{
		breadth_first_search,
		a_star
	};

	struct search_algorithms_specification
	{
		rendering::window_specification window_specification;
		rendering::grid_renderer_specification grid_renderer_specification;

		search_algorithms_specification() = default;

		search_algorithms_specification(const rendering::window_specification&& window_specification,
			const rendering::grid_renderer_specification& grid_renderer_specification) :
			window_specification(window_specification), grid_renderer_specification(grid_renderer_specification)
		{
		}
	};

	class search_algorithms
	{
	public:
		search_algorithms(const search_algorithms_specification& specification);
		~search_algorithms();

		void run();

		static search_algorithms& get_application();

		const unique<rendering::window>& get_window() { return m_window; }

	private:
		void initialize_window();
		void initialize_camera();
		void initialize_renderer();
		void initialize_ui();

		application_state m_state;
		algorithm_type m_algorithm;
		search_algorithms_specification m_specification;

		unique<rendering::grid_renderer> m_grid_renderer;

		unique<rendering::camera> m_camera;
		glm::vec3 m_camera_pos = { 50 / 2, 50 / 2, 26.0f };

		unique<rendering::window> m_window;

		unique<ui::ui> m_user_interface;

		glm::vec2 m_start_location;
		glm::vec2 m_end_location;

		static search_algorithms* s_instance;
	};
}
