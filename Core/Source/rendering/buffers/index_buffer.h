#pragma once
#include "rendering/graphics_object.h"

namespace rendering {
    class index_buffer : public graphics_object {
    public:
        /* Constructor & Destructor */
        index_buffer(unsigned int* indices, unsigned int count);
        ~index_buffer() override;

        /* Methods */
        void bind() override;
        void un_bind() override;

        unsigned int get_size() const;

    private:
        unsigned int m_size;
    };
}
