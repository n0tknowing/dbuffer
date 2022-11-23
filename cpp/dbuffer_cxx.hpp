#include <climits>
#include <stdint.h>

class DBuffer {
public:
    DBuffer(size_t m_capacity = 8192);
    ~DBuffer() noexcept;

    // Disable and never use this to prevents leak
    DBuffer(const DBuffer&) = delete;
    DBuffer& operator=(const DBuffer&) = delete;

    // Disable until I understand them better
    DBuffer(const DBuffer&&) = delete;
    DBuffer&& operator=(const DBuffer&&) = delete;

    void grow(const size_t new_capacity);
    void shrink_to_fit();
    void push(const uint8_t *data, const size_t size);
    void clear() noexcept;

    [[nodiscard]] bool operator==(const DBuffer& other) noexcept;

    [[nodiscard]] size_t size() noexcept {
        return m_size;
    }

    [[nodiscard]] size_t capacity() noexcept {
        return m_capacity;
    }

    [[nodiscard]] uint8_t *buffer() noexcept {
        return m_buffer;
    }

    [[nodiscard]] const uint8_t *buffer() const noexcept {
        return static_cast<const uint8_t *>(m_buffer);
    }

private:
    uint8_t *m_buffer = nullptr;
    size_t m_size = 0;
    size_t m_capacity = 0; // soft capacity limit
    static constexpr size_t MAX_CAPACITY_HARD = LONG_MAX;
};
