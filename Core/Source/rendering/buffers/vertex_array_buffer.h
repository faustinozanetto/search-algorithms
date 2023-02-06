#pragma once

#include "rendering/buffers/index_buffer.h"
#include "rendering/buffers/vertex_object_buffer.h"
#include "rendering/graphics_object.h"

namespace rendering
{
    class vertex_array_buffer : public graphics_object
    {
    public:
        /* Constructor & Destructor */
        vertex_array_buffer();
        ~vertex_array_buffer() override;

        /* Methods */
        void bind() override;
        void un_bind() override;

        void add_vertex_buffer(const shared<vertex_object_buffer>& vbo);
        const std::vector<shared<vertex_object_buffer>>& get_vertex_buffers() const;
        const shared<index_buffer>& get_index_buffer() const;
        void set_index_buffer(const shared<index_buffer>& ibo);

    private:
        std::vector<shared<vertex_object_buffer>> m_vbos;
        shared<index_buffer> m_ibo;
    };
}
