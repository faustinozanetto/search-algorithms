#pragma once

#include "glfw/glfw3.h"

namespace ui
{
	class ui
	{
	public:
		ui();
		~ui();

		void initialize(GLFWwindow* window_handle);
		void terminate();

		void being_frame_render();
		void end_frame_render();

		void initialize_dockspace();
	};
}
