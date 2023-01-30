#pragma once

#include "iostream"

#include "include/glfw/glfw3.h"

namespace rendering
{
	struct window_specification
	{
		std::string title;
		int width;
		int height;
		bool v_sync;

		window_specification(const std::string& title = "Search Algorithms", int width = 1920,
			int height = 1080, bool v_sync = true) : title(title), width(width),
			height(height), v_sync(v_sync)
		{
		}
	};

	class window
	{
	public:
		window(const window_specification& specification);
		~window();

		void set_vsync_enabled(bool use_vsync);

		void* get_native_window() const;
		window_specification& get_window_specification() { return m_window_specification; };

		static unique<window> create(const window_specification& specification);

	private:
		bool initialize_window();

	protected:
		window_specification m_window_specification;

		GLFWwindow* m_window_handle;
	};
}
