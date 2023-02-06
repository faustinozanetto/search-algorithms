#include "pch.h"

#include <glad/glad.h>

#include "rendering/window/window.h"
#include <assert.h>

namespace rendering
{
	window::window(const window_specification& specification)
	{
		m_window_specification = specification;
	}

	window::~window()
	{
		glfwDestroyWindow(m_window_handle);
		glfwTerminate();
	}

	void window::set_vsync_enabled(bool use_vsync)
	{
		// OpenGL Vsync implementation.
		if (use_vsync)
			glfwSwapInterval(1);
		else glfwSwapInterval(0);
		m_window_specification.v_sync = use_vsync;
	}

	void window::setup_callbacks()
	{
		glfwSetWindowSizeCallback(m_window_handle, [](GLFWwindow* window, int width, int height)
			{
				window_specification& windowSpecification = *static_cast<window_specification*>(
					glfwGetWindowUserPointer(window));
		windowSpecification.width = width;
		windowSpecification.height = height;
			});
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
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

		// Creating the window
		m_window_handle = glfwCreateWindow(m_window_specification.width,
			m_window_specification.height, m_window_specification.title.c_str(),
			nullptr,
			nullptr);

		assert(m_window_handle, "Could not create the glfw window!");

		// Set glfw context
		glfwMakeContextCurrent(m_window_handle);

		// Set user pointer, used in callbacks.
		glfwSetWindowUserPointer(m_window_handle, &m_window_specification);

		// Glad initialize
		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
			return false;

		// Handle Initial VSync.
		if (m_window_specification.v_sync)
			set_vsync_enabled(true);
		else set_vsync_enabled(false);

		return true;
	}

	unique<window> window::create(const window_specification& specification)
	{
		return create_unique<window>(specification);
	}
}
