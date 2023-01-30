#include "pch.h"

#include "include/glad/glad.h"
#include "rendering/renderer/renderer.h"

namespace rendering
{
  renderer::renderer(const renderer_specification &specification)
  {
    m_renderer_speficiaction = specification;
  }

  renderer::~renderer()
  {
  }

  bool renderer::initialize()
  {
    // Set glfw context
    glfwMakeContextCurrent(m_renderer_speficiaction.window_handle);

    // Glad initialize
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
      return false;

    return true;
  }

  void renderer::swap_buffers()
  {
    glfwSwapBuffers(m_renderer_speficiaction.window_handle);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  bool renderer::should_close()
  {
    return glfwWindowShouldClose(m_renderer_speficiaction.window_handle);
  }

  unique<renderer> renderer::create(const renderer_specification &specification)
  {
    return create_unique<renderer>(specification);
  }
}
