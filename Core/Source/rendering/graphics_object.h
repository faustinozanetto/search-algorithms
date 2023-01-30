#pragma once

namespace rendering {
class graphics_object {
public:
  ~graphics_object() = default;

  virtual void bind() = 0;
  virtual void un_bind() = 0;

  unsigned int get_object_handle() const { return m_object_handle; }

protected:
  unsigned int m_object_handle;
};
} // namespace rendering
