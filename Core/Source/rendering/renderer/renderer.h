#pragma once

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
		static bool initialize(const renderer_specification& specification);

		static void swap_buffers();
		static void poll_input();
		static void set_clear_color(glm::vec4 color);
		static void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void clear_screen();
		static bool should_close();

		static void render_instanced(unsigned int count, unsigned int instance_count);

	private:
		static renderer_specification s_renderer_specification;
	};
}
