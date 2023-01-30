#include "pch.h"

#include <iostream>
#include "rendering/shader/shader.h"
#include <rendering/window/window.h>

int main()
{
	std::cout << "Hello World" << std::endl;

	unique<rendering::window> window = rendering::window::create({});

	shared<rendering::shader> shader = rendering::shader::create({ "Assets/Shaders/test.vert", "Assets/Shaders/test.frag" });
	std::cout << shader->get_object_handle() << std::endl;

	while (true) {
	}

	return 0;
}
