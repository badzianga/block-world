#include <chrono>
#include <iostream>
#include "world/Chunk.hpp"
#include "world/Generator.hpp"

Chunk Generator::generate() {
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    std::array<uint8_t, Config::Rendering::chunkVolume> blocks{};

    for (int z = 0; z < Config::Rendering::chunkSize; ++z) {
        for (int y = 0; y < Config::Rendering::chunkSize; ++y) {
            for (int x = 0; x < Config::Rendering::chunkSize; ++x) {
                int type = 0;

                if (y == Config::Rendering::chunkSize - 1) {
                    type = 0;
                }
                else if (y > Config::Rendering::chunkSize - 3) {
                    type = 2;
                }
                else if (y > Config::Rendering::chunkSize * 3 / 5) {
                    type = 1;
                }
                else {
                    type = 3;
                }

                blocks[z * Config::Rendering::chunkArea + y * Config::Rendering::chunkSize + x] = type;
            }
        }
    }

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Chunk generated in " << elapsed_seconds.count() << " seconds\n";

    return Chunk(blocks);
}
