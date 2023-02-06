#include "pch.h"

#include <glad/glad.h>

#include "rendering/buffers/vertex_object_buffer.h"

namespace rendering {
    layout_element::layout_element(layout_element_type type, std::string elementName)
    {
        this->type = type;
        name = elementName;
        size = CalculateElementSize(type);
        offset = 0;
    }

    uint32_t layout_element::calculate_element_count() const
    {
        switch (type)
        {
        case Float: return 1;
        case FloatVec2: return 2;
        case FloatVec3: return 3;
        case FloatVec4: return 4;
        }
        return 0;
    }

    std::string layout_element::get_element_type_name(layout_element_type type)
    {
        switch (type)
        {
        case Float: return "Float";
        case FloatVec2: return "FloatVec2";
        case FloatVec3: return "FloatVec3";
        case FloatVec4: return "FloatVec4";
        }
        return "Unknown";
    }

    vertex_buffer_layout::vertex_buffer_layout(std::initializer_list<layout_element> elements)
    {
        m_elements = elements;
        calculate_offset_and_stride();
    }

    void vertex_buffer_layout::calculate_offset_and_stride()
    {
        int offset = 0;
        m_stride = 0;
        for (auto& vboElement : m_elements)
        {
            vboElement.offset = offset;
            offset += vboElement.size;
            m_stride += vboElement.size;
        }
    }

    const std::vector<layout_element>& vertex_buffer_layout::get_elements() const
    {
        return m_elements;
    }

    uint32_t vertex_buffer_layout::get_stride() const
    {
        return m_stride;
    }

    vertex_object_buffer::vertex_object_buffer(const float* vertices, unsigned int size)
    {
        glCreateBuffers(1, &m_object_handle);
        glBindBuffer(GL_ARRAY_BUFFER, m_object_handle);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    vertex_object_buffer::~vertex_object_buffer()
    {
        glDeleteBuffers(1, &m_object_handle);
    }

    void vertex_object_buffer::bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_object_handle);
    }

    void vertex_object_buffer::un_bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void vertex_object_buffer::set_data(const void* data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_object_handle);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    const vertex_buffer_layout& vertex_object_buffer::get_layout() const
    {
        return m_vertex_buffer_layout;
    }

    void vertex_object_buffer::set_layout(const vertex_buffer_layout& layout)
    {
        m_vertex_buffer_layout = layout;
    }

    uint32_t vertex_object_buffer::get_layout_element_type(layout_element_type vboElementType)
    {
        switch (vboElementType)
        {
        case Float: return GL_FLOAT;
        case FloatVec2: return GL_FLOAT;
        case FloatVec3: return GL_FLOAT;
        case FloatVec4: return GL_FLOAT;
        }
        return NULL;
    }
}
