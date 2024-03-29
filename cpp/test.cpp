#include <array>
#include <iostream>
#include <memory>
#include <cstring>

#include "dbuffer.h"

int main() {
    DBuffer buffer;

    std::array<size_t, 6> sizes{20'000, 60'000, 64, 1'000, 5'000, 8192};

    for (size_t i = 0; i < sizes.size(); i++) {
        uint8_t *buf = new uint8_t[sizes[i]];
        std::memset(buf, i, sizes[i]);
        buffer.put(buf, sizes[i]);
        std::cout << ".push() " << sizes[i] << " bytes success\n";
        delete[] buf;
    }

    std::cout << '\n';
    std::cout << ".size() " << buffer.size() << " bytes\n";
    std::cout << ".capacity() " << buffer.capacity() << " bytes\n";
    std::cout << '\n';

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

    {
        DBuffer buffer2 {std::move(buffer)};

        std::cout << '\n';
        std::cout << ".size() " << buffer2.size() << " bytes\n";
        std::cout << ".capacity() " << buffer2.capacity() << " bytes\n";
    }

    std::cout << '\n';
    std::cout << ".size() " << buffer.size() << " bytes\n";
    std::cout << ".capacity() " << buffer.capacity() << " bytes\n";

    buffer.resize(8);
    std::cout << '\n';
    std::cout << ".size() " << buffer.size() << " bytes\n";
    std::cout << ".capacity() " << buffer.capacity() << " bytes\n";

    const uint8_t buf[10] = {1,2,3,4,5,6,7,8,9,0};
    buffer.put(buf, 10);
    std::cout << '\n';
    std::cout << ".size() " << buffer.size() << " bytes\n";
    std::cout << ".capacity() " << buffer.capacity() << " bytes\n";

    {
        DBuffer buffer2;
        buffer2.put({1,2,3,4,5,6,7,8,9,0});
        std::cout << '\n';
        if (buffer == buffer2)
            std::cout << "OK\n";
        else
            std::cout << "WTF???\n";
    }

    return 0;
}
