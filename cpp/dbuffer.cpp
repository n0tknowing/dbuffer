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

DBuffer& DBuffer::operator=(DBuffer&& other) {
    if (this != &other) {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        std::swap(m_buffer, other.m_buffer);
        delete[] other.m_buffer;
        other.m_buffer = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }
    return *this;
}

DBuffer::~DBuffer() noexcept {
    if (m_buffer != nullptr) {
        delete[] m_buffer;
        m_buffer = nullptr;
        m_size = 0;
        m_capacity = 0;
    }
}

void DBuffer::resize(const size_t new_capacity) {
    if (new_capacity == 0 || new_capacity == m_capacity)
        return;
    else if (m_capacity >= MAX_CAPACITY_HARD)
        throw std::runtime_error("Out of memory");

    const size_t new_cap = std::min(new_capacity, MAX_CAPACITY_HARD);
    uint8_t *new_data = new uint8_t[new_cap]();

    if (m_buffer != nullptr) {
        std::memcpy(new_data, m_buffer, std::min(m_size, new_cap));
        delete[] m_buffer;
    }

    if (new_cap < m_size)
        m_size = new_cap;

    m_buffer = new_data;
    m_capacity = new_cap;
}

void DBuffer::put(const uint8_t *data, const size_t size) {
    if (size == 0)
        return;

    const size_t needed_size = std::min(size, MAX_CAPACITY_HARD);
    const size_t diff = m_capacity - m_size;
    if (needed_size >= diff)
        DBuffer::resize(std::max(needed_size * 2, m_capacity * 2));

    std::memcpy(m_buffer, data, needed_size);
    m_size += needed_size;
}

void DBuffer::put(std::initializer_list<uint8_t> list) {
    DBuffer::put(std::data(list), list.size());
}

void DBuffer::clear() noexcept {
    if (m_buffer != nullptr) {
        std::memset(m_buffer, 0, m_size);
        m_size = 0;
    }
}

[[nodiscard]] bool DBuffer::operator==(const DBuffer &other) const noexcept {
    if (this == &other)
        return true;
    else if (m_size != other.m_size)
        return false;

    return std::memcmp(m_buffer, other.m_buffer, m_size) == 0;
}

[[nodiscard]] bool DBuffer::operator!=(const DBuffer &other) const noexcept {
    return !(*this == other);
}
