#include "pch.h"

#include <GLFW/glfw3.h>
#include "core/search_algorithms.h"
#include "input/input_manager.h"

namespace input
{
	bool input_manager::is_key_pressed(key_code key)
	{
		auto window = static_cast<GLFWwindow*>(search_algorithms::get_application().get_window()->get_native_window());
		int state = glfwGetKey(window, key);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool input_manager::is_mouse_button_pressed(mouse_key_code mouse_key)
	{
		auto window = static_cast<GLFWwindow*>(retro_application::get_application().get_window()->get_native_window());
		int state = glfwGetMouseButton(window, mouse_key);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	glm::vec2 input_manager::get_mouse_location()
	{
		auto* window = static_cast<GLFWwindow*>(retro_application::get_application().get_window()->get_native_window());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}
}
