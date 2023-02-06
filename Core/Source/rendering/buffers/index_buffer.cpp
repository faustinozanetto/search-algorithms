#include "pch.h"

#include "glad/glad.h"
#include "rendering/buffers/index_buffer.h"

namespace rendering
{
    index_buffer::index_buffer(unsigned int* indices, unsigned int count)
    {
        // Assign size
        m_size = count;
        glCreateBuffers(1, &m_object_handle);
        glBindBuffer(GL_ARRAY_BUFFER, m_object_handle);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    }

    index_buffer::~index_buffer()
    {
        // Delete buffer
        glDeleteBuffers(1, &m_object_handle);
    }

    void index_buffer::bind()
    {
        // Bind the buffer.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_object_handle);
    }

    void index_buffer::un_bind()
    {
        // Unbind the buffer.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    unsigned int index_buffer::get_size() const
    {
        return m_size;
    }
}
