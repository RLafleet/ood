#include <iostream>
#include <fstream>
#include "SVGCanvas.h"
#include "CDesigner.h"
#include "Painter.h"
#include "Client.h"
#include "ShapeFactory.h"

const std::string COMMAND_INPUT_ARGS = "cin";

const int CANVAS_WIDTH = 8000;
const int CANVAS_HEIGHT = 8000;

struct Args
{
    std::string inputFileName;
};

Args ParseArgs(int argc, char* argv[])
{
    if (argc != 2)
    {
        throw std::invalid_argument("Invalid arguments count\nUsage: <input_name>");
    }

    Args args;
    args.inputFileName = argv[1];
    return args;
}

std::ifstream GetInputFile(const std::string& inputFileName)
{
    std::ifstream inputFile(inputFileName);
    return inputFile;
}

void CheckOpenInputStream(std::ifstream& inputFile)
{
    if (!inputFile.is_open())
    {
        throw std::runtime_error("Failed to open input stream");
    }
}

int main(int argc, char* argv[])
{
    try
    {
        Args args = ParseArgs(argc, argv);

        gfx::SVGCanvas canvas(CANVAS_WIDTH, CANVAS_HEIGHT);
        ShapeFactory shapeFactory;
        CDesigner designer(shapeFactory);
        Painter painter;
        Client client(designer);

        if (args.inputFileName != COMMAND_INPUT_ARGS)
        {
            std::ifstream fileInput = GetInputFile(args.inputFileName);
            CheckOpenInputStream(fileInput);

            if (fileInput.peek() == std::ifstream::traits_type::eof())
            {
                std::cout << "Error reading input file: file is empty" << std::endl;
                return 0;
            }

            client.HandleCommand(fileInput, canvas, painter);
        }
        else
        {
            client.HandleCommand(std::cin, canvas, painter);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}