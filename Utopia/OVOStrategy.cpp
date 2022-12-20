#include "OVOStrategy.h"
#include "OVOFactory.h"
#include <iostream>
#include <fstream>

using namespace utopia;

std::unique_ptr<OVOStrategy::Buffer> OVOStrategy::getChunksByte(std::ifstream& inFile)
{

    unsigned int chunkSize = 0;
    inFile.read(reinterpret_cast<char*>(&chunkSize), sizeof(unsigned int));
    auto buffer = std::make_unique<OVOStrategy::Buffer>(chunkSize);
    

    if (!inFile.read(buffer->data.get(), chunkSize))
    {
        inFile.close();
        throw std::exception("Cannot read a number of bytes equal to the chunk_size interpreted");
    }

    return std::move(buffer);
}