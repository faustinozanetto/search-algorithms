#include "pch.h"

#include <iostream>
#include "rendering/shader/shader.h"
#include <rendering/window/window.h>
#include <rendering/renderer/renderer.h>

int main()
{
	std::cout << "Hello World" << std::endl;
	unique<rendering::window> window = rendering::window::create({});
	unique<rendering::renderer> renderer = rendering::renderer::create({ window->get_native_window() });
	renderer->initialize();

	shared<rendering::shader> shader = rendering::shader::create({ "Assets/Shaders/test.vert", "Assets/Shaders/test.frag" });
	std::cout << shader->get_object_handle() << std::endl;

	while (!renderer->should_close())
	{
		renderer->clear_screen();
		renderer->set_clear_color({ 0.25, 0.35, 0.45, 1.0 });
		renderer->swap_buffers();
		renderer->poll_input();
	}

	return 0;
}
