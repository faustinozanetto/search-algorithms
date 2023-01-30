#include "pch.h"

#include "rendering/window/window.h"

namespace rendering
{
	window::window(const window_specification& specification)
	{
		m_window_specification = specification;
		initialize_window();
	}

	window::~window()
	{
		glfwDestroyWindow(m_window_handle);
		glfwTerminate();
	}

	void window::set_vsync_enabled(bool use_vsync)
	{
		// OpenGL Vsync implementation.
		glfwSwapInterval(use_vsync ? 1 : 0);
		m_window_specification.v_sync = use_vsync;
	}

	GLFWwindow* window::get_native_window() const
	{
		return m_window_handle;
	}

	bool window::initialize_window()
	{
		// Initializing GLFW
		int success = glfwInit();
		if (success != GLFW_TRUE)
			return false;

		// Setup window hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		glfwWindowHint(GLFW_SAMPLES, 8);

		// Creating the window
		m_window_handle = glfwCreateWindow(m_window_specification.width,
			m_window_specification.height, m_window_specification.title.c_str(),
			nullptr,
			nullptr);

		// Set user pointer, used in callbacks.
		glfwSetWindowUserPointer(m_window_handle, &m_window_specification);

		// Handle Initial VSync.
		set_vsync_enabled(true);

		return true;
	}

	unique<window> window::create(const window_specification& specification)
	{
		return create_unique<window>(specification);
	}
}
