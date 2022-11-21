#include <array>
#include <iostream>
#include <memory>
#include <cstring>

#include "dbuffer_cxx.hpp"

int main() {
    DBuffer buffer;

    std::array<size_t, 6> sizes{20'000, 60'000, 64, 1'000, 5'000, 8192};

    for (size_t i = 0; i < sizes.size(); i++) {
        uint8_t *buf = new uint8_t[sizes[i]];
        std::memset(buf, i, sizes[i]);
        buffer.push(buf, sizes[i]);
        std::cout << ".push() " << sizes[i] << " bytes success\n";
        delete[] buf;
    }

    std::cout << ".size() " << buffer.size() << " bytes\n";
    std::cout << ".capacity() " << buffer.capacity() << " bytes\n";

    {
        DBuffer buffer2;

        uint8_t *buf = new uint8_t[4096];
        std::memset(buf, 10, 4096);

        if (buffer == buffer2)
            std::cout << "WTF???\n";
        else
            std::cout << "OK\n";

        delete[] buf;
    }

    return 0;
}
