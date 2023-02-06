#include "pch.h"

#include "glad/glad.h"
#include "rendering/buffers/vertex_array_buffer.h"

namespace rendering
{
	vertex_array_buffer::vertex_array_buffer()
	{
		// Create vertex array.
		glGenVertexArrays(1, &m_object_handle);
		// Bind the vertex array
		glBindVertexArray(m_object_handle);
	}

	vertex_array_buffer::~vertex_array_buffer()
	{
		glDeleteVertexArrays(1, &m_object_handle);
	}

	void vertex_array_buffer::bind()
	{
		glBindVertexArray(m_object_handle);
	}

	void vertex_array_buffer::un_bind()
	{
		glBindVertexArray(0);
	}

	void vertex_array_buffer::add_vertex_buffer(const shared<vertex_object_buffer>& vbo)
	{
		// Bind this VAO and then bind the VBO.
		bind();
		vbo->bind();

		const vertex_buffer_layout& vboLayout = vbo->get_layout();
		int index = 0;
		for (const layout_element& vboElement : vboLayout.get_elements())
		{
			// Setup according to type.
			switch (vboElement.type)
			{
			case Float:
			case FloatVec2:
			case FloatVec3:
			case FloatVec4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index,
					vboElement.calculate_element_count(),
					vertex_object_buffer::get_layout_element_type(vboElement.type),
					GL_FALSE,
					vboLayout.get_stride(),
					reinterpret_cast<const void*>(vboElement.offset));
				// Increment the index
				index++;
				break;
			}
			case Int:
			case IntVec2:
			case IntVec3:
			case IntVec4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index,
					vboElement.calculate_element_count(),
					vertex_object_buffer::get_layout_element_type(vboElement.type),
					vboLayout.get_stride(),
					reinterpret_cast<const void*>(vboElement.offset));
				// Increment the index
				index++;
				break;
			}
			case Mat3:
			case Mat4:
			{
				// Get count and loop
				const uint32_t size = vboElement.calculate_element_count();
				for (uint8_t i = 0; i < size; i++)
				{
					// Enable the attribute
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index, size,
						vertex_object_buffer::get_layout_element_type(vboElement.type),
						GL_FALSE, vboLayout.get_stride(),
						reinterpret_cast<const void*>(sizeof(float) * size * i));
					glVertexAttribDivisor(index, 1);
					// Increment the index
					index++;
				}
				break;
			}
			}
		}

		m_vbos.push_back(vbo);
	}

	const std::vector<shared<vertex_object_buffer>>& vertex_array_buffer::get_vertex_buffers() const
	{
		return m_vbos;
	}

	const shared<index_buffer>& vertex_array_buffer::get_index_buffer() const
	{
		return m_ibo;
	}

	void vertex_array_buffer::set_index_buffer(const shared<index_buffer>& ibo)
	{
		bind();
		ibo->bind();
		m_ibo = ibo;
	}
}
