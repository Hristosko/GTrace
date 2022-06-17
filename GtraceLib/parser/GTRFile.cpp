#include "GTRFile.h"
#include "FileWriter.h"
#include "FileReader.h"
#include "common/Logger.h"
#include <iostream>

namespace gtrace
{
RendererOutput GTRFile::parse(const char* filePath)
{
    LOGINFO("Start parsing gtr file: ", filePath);
    RendererOutput output;
    BinFileReader file(filePath);

    const auto version = file.read<uint32_t>();

    const auto width = file.read<uint64_t>();
    const auto height = file.read<uint64_t>();
    output.setSize(width, height);

    const auto outputsCount = file.read<uint64_t>();

    for (uint64_t i = 0; i < outputsCount; ++i)
    {
        const auto nameSize = file.read<uint64_t>();
        std::string name(nameSize, '\0');
        file.read(&name[0], nameSize);
        assert(name.length() == nameSize);

        const auto elementSize = file.read<uint64_t>();
        const auto elementsCount = file.read<uint64_t>();

        DataBuffer data;
        data.init(elementSize, elementsCount);

        file.read(data.getAt(0), elementSize, elementsCount);

        output.addOutput(name, std::move(data));
    }

    LOGINFO("Parsed gtr file: ", filePath);
    for (const auto& out : output)
        LOGINFO("Parsed output ", out.first);

    return output;
}

void GTRFile::dump(const RendererOutput& output, const char* filePath)
{
    LOGINFO("Writing gtr file: ", filePath);

    BinFileWriter file(filePath);
    file.write<uint32_t>(&CURRENT_VERSION, 1);

    const auto width = output.getWidth();
    const auto height = output.getHeight();
    file.write<uint64_t>(&width, 1);
    file.write<uint64_t>(&height, 1);

    uint64_t outputsCount = output.count();
    file.write<uint64_t>(&outputsCount, 1);

    for (const auto& [name, data] : output)
    {
        const uint64_t nameSize = name.size();
        file.write(&nameSize, 1);
        file.write(name.data(), nameSize);

        const uint64_t elementSize = data.getElementSize();
        const uint64_t elementsCount = data.getSize();

        file.write(&elementSize, 1);
        file.write(&elementsCount, 1);
        file.write(data.getAt(0), elementSize, elementsCount);
    }

    LOGINFO("Finished writing gtr file: ", filePath);
}

}  // namespace gtrace
