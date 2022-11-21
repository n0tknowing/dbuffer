#include <algorithm>
#include <memory>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <cstdio>

#include "dbuffer_cxx.hpp"

DBuffer::DBuffer(size_t init_capacity) {
	m_capacity = std::min(init_capacity, MAX_CAPACITY_HARD);
	m_buffer = new uint8_t[m_capacity];
	std::memset(m_buffer, 0, m_capacity * sizeof(uint8_t));
}

DBuffer::~DBuffer() noexcept {
	delete[] m_buffer;
	m_buffer = nullptr;
	m_size = 0;
	m_capacity = 0;
}

void DBuffer::grow(const size_t new_capacity) {
	if (new_capacity <= m_capacity)
		return;
	else if (m_capacity >= MAX_CAPACITY_HARD)
		throw std::runtime_error("Out of memory");

	const size_t new_cap = std::min(new_capacity, MAX_CAPACITY_HARD);
	uint8_t *new_data = new uint8_t[new_cap];

	std::memset(new_data, 0, new_cap); // zeroes entire new buffer
	std::memcpy(new_data, m_buffer, m_size); // copy old data

	delete[] m_buffer; // delete old buffer after we copy it

	m_buffer = new_data; // new buffer with old data + 0
	m_capacity = new_cap;
}

void DBuffer::shrink() {
	if (m_size == 0 || m_size == m_capacity)
		return;

	uint8_t *new_buffer = new uint8_t[m_size];
	std::memcpy(new_buffer, m_buffer, m_size); // copy old content

	delete[] m_buffer; // delete

	m_buffer = new_buffer;
	m_capacity = m_size;
}

void DBuffer::push(const uint8_t *data, const size_t size) {
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
