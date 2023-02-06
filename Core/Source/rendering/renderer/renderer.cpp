#include "pch.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "rendering/renderer/renderer.h"

namespace rendering
{
	renderer_specification renderer::s_renderer_specification = {};

	bool renderer::initialize(const renderer_specification& specification)
	{
		s_renderer_specification = specification;
		return true;
	}

	void renderer::swap_buffers()
	{
		glfwSwapBuffers(s_renderer_specification.window_handle);
	}

	void renderer::poll_input()
	{
		glfwPollEvents();
	}

	void renderer::set_clear_color(glm::vec4 color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void renderer::set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void renderer::clear_screen()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	bool renderer::should_close()
	{
		return glfwWindowShouldClose(s_renderer_specification.window_handle);
	}

	void renderer::render_instanced(unsigned count, unsigned instance_count)
	{
		glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0, instance_count);
	}
}
