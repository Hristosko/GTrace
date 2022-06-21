#include "common/Logger.h"
#include "common/Errors.h"
#include "common/MemoryTracker.h"
#include "renderer/Renderer.h"
#include "parser/GTRFile.h"

#include <stdio.h>
#include <vector>
#include <string.h>

using namespace gtrace;

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

static uint32_t totalBucketsCount = 0;

static void updateProgress()
{
    static uint32_t renderedBucketsCount = 0;
    if (totalBucketsCount <= 0)
        return;

    ++renderedBucketsCount;
    // we update the render surface once again when everything is rendered
    if (renderedBucketsCount > totalBucketsCount)
        return;

    printf("\r");
    printf("[%u/%u] %u%%", renderedBucketsCount, totalBucketsCount, renderedBucketsCount * 100 / totalBucketsCount);

    if (renderedBucketsCount == totalBucketsCount)
        printf("\n");
}

struct Operation
{
    virtual void excecute() const = 0;
};

struct RenderOperation : Operation
{
    RenderOperation(const std::string& inputFile, const std::string& outputFile) :
        inputFile(inputFile),
        outputFile(outputFile)
    {
    }
    std::string inputFile;
    std::string outputFile;

    virtual void excecute() const
    {
        printf("Rendering scene...\n");
        RendererOutput output;
        Scene scene;
        scene.sceneSettings = {DEFAULT_WIDTH, DEFAULT_HEIGHT};
        Renderer renderer(std::move(scene), &output, updateProgress);
        renderer.render();

        printf("Storing ouput...\n");
        GTRFile::dump(output, outputFile.c_str());
    }
};

std::unique_ptr<Operation> parse(const std::vector<std::string>& arguments)
{
    if (arguments.empty())
        Raise(InalidArgument("GTrace console app requires at least one argument"));

    if (arguments[0] == "--render")
    {
        if (arguments.size() != 3)
            Raise(InalidArgument("To render a scene specify input and output file"));
        return std::make_unique<RenderOperation>(arguments[1], arguments[2]);
    }

    Raise(InalidArgument("Unknown argument " + arguments[0]));
}

int main(int argc, char** argv)
{
    MemoryTracker::reset();
    {
        std::vector<std::string> arguments;
        for (int i = 1; i < argc; ++i)
            arguments.push_back(argv[i]);

        parse(arguments)->excecute();
    }
    MemoryTracker::logStatistic();
    return 0;
}
