#include "PPMFile.h"
#include "FileWriter.h"
#include "common/Logger.h"
#include "common/Errors.h"

namespace gtrace
{
static void writeHeader(const BinFileWriter& file, const std::string& header)
{
    file.write<char>(header.c_str(), header.size());
}

void PPMFile::dump(const DataBuffer& data, uint64_t width, uint64_t height, const char* filePath)
{
    LOGINFO("Writing PPM file: ", filePath);

    if (data.getElementSize() != sizeof(unsigned char))
        Raise(InvalidDataBuffer(
            "Expected Color3f buffer. Actual element size is: " + std::to_string(data.getElementSize())));

    if (data.getSize() != 3 * width * height)
        Raise(InvalidDataBuffer(
            "Unexpected data buffer size. Actual size is: " + std::to_string(data.getSize()) +
            " . Expected size is 3*" + std::to_string(width) + '*' + std::to_string(height)));

    BinFileWriter file(filePath);

    writeHeader(file, "P6\n");
    writeHeader(file, std::to_string(width) + ' ' + std::to_string(height) + '\n');
    writeHeader(file, "255\n");

    file.write(data.getAt<unsigned char>(0), data.getSize());
}

}  // namespace gtrace