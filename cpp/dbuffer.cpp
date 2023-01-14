#include <algorithm>
#include <memory>
#include <cstring>
#include <stdexcept>

#include "dbuffer.h"

DBuffer::DBuffer(size_t init_capacity) {
    m_capacity = std::min(init_capacity, MAX_CAPACITY_HARD);
    m_buffer = new uint8_t[m_capacity]();
}

DBuffer::DBuffer(DBuffer&& other) {
    if (this != &other) {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        std::swap(m_buffer, other.m_buffer);
        delete[] other.m_buffer;
        other.m_buffer = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }
}

DBuffer::~DBuffer() noexcept {
    if (m_buffer != nullptr) {
        delete[] m_buffer;
        m_buffer = nullptr;
        m_size = 0;
        m_capacity = 0;
    }
}

void DBuffer::grow(const size_t new_capacity) {
    if (new_capacity <= m_capacity)
        return;
    else if (m_capacity >= MAX_CAPACITY_HARD)
        throw std::runtime_error("Out of memory");

    const size_t new_cap = std::min(new_capacity, MAX_CAPACITY_HARD);
    uint8_t *new_data = new uint8_t[new_cap]();

    std::memcpy(new_data, m_buffer, m_size); // copy old data
    delete[] m_buffer; // delete old buffer after we copy it

    m_buffer = new_data; // new buffer with old data + a bunch of zero
    m_capacity = new_cap;
}

void DBuffer::shrink_to_fit() {
    if (m_size == 0 || m_size == m_capacity)
        return;

    uint8_t *new_buffer = new uint8_t[m_size];
    std::memcpy(new_buffer, m_buffer, m_size); // copy old content

    delete[] m_buffer;

    m_buffer = new_buffer;
    m_capacity = m_size;
}

void DBuffer::put(const uint8_t *data, const size_t size) {
    const size_t needed_size = std::min(size, MAX_CAPACITY_HARD);
    const size_t diff = m_capacity - m_size; // grow or not?
    if (needed_size >= diff)
        DBuffer::grow(std::max(needed_size * 2, m_capacity * 2));

    std::memcpy(m_buffer, data, needed_size);
    m_size += needed_size;
}

void DBuffer::clear() noexcept {
    std::memset(m_buffer, 0, m_size);
    m_size = 0;
}

[[nodiscard]] bool DBuffer::operator==(const DBuffer &other) const noexcept {
    // avoid wasting memcmp call because we only compare if
    // both addresses are unique...
    if (this == &other)
        return true;
    else if (m_size != other.m_size) // ...and have same size
        return false;

    return std::memcmp(m_buffer, other.m_buffer, m_size) == 0;
}

[[nodiscard]] bool DBuffer::operator!=(const DBuffer &other) const noexcept {
    return !(*this == other);
}
