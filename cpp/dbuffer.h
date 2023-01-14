#include <climits>
#include <stdint.h>

class DBuffer {
public:
    DBuffer(size_t m_capacity = 8192);
    DBuffer(DBuffer&&);
    DBuffer& operator=(DBuffer&&);
    ~DBuffer() noexcept;

    DBuffer(const DBuffer&) = delete;
    DBuffer& operator=(const DBuffer&) = delete;

    void resize(const size_t new_capacity);
    void put(const uint8_t *data, const size_t size);
    void clear() noexcept;

    [[nodiscard]] bool operator==(const DBuffer& other) const noexcept;
    [[nodiscard]] bool operator!=(const DBuffer& other) const noexcept;

    [[nodiscard]] size_t size() const noexcept {
        return m_size;
    }

    [[nodiscard]] size_t capacity() const noexcept {
        return m_capacity;
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
