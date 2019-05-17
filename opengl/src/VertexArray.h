#pragma once

#include "vertex_buffer.h"

class VertexArray
{
private:

public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};