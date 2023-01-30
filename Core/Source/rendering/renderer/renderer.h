#pragma once

#include <include/GLFW/glfw3.h>
#include "glm/glm.hpp"

namespace rendering
{
	struct renderer_specification
	{
		GLFWwindow* window_handle;

		renderer_specification() = default;

		renderer_specification(GLFWwindow* handle) : window_handle(handle)
		{
		}
	};

	class renderer
	{
	public:
		renderer(const renderer_specification& specification);
		~renderer();

		bool initialize();

		void swap_buffers();
		void poll_input();
		void set_clear_color(glm::vec4 color);
		void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		void clear_screen();
		bool should_close();

		static unique<renderer> create(const renderer_specification& specification);

	private:
		renderer_specification m_renderer_speficiaction;
	};
}
