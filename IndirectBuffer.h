#pragma once

class IndirectBuffer {
public:
    virtual void setData(const void* data, size_t size) = 0;
    virtual void updateData(const void* data, size_t size, size_t offset = 0) = 0;
    virtual void bind() = 0;
    virtual ~IndirectBuffer() {}
};