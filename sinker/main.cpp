#include <cstdlib>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <istream>
#include <CLI/CLI.hpp>

#include "sinker.hpp"

int main(int argc, char const *argv[])
{
    CLI::App app{"Sinker Compiler"};
    std::string output_filename;
    std::string def_filename;
    std::vector<std::string> input_filenames;
    bool debug = false;
    app.add_option("-o,--output", output_filename, "Output file");
    app.add_option("-d,--def", def_filename, "Definitions file");
    app.add_option("input_files", input_filenames, "Input files");
    app.add_flag("--debug,!--no-debug", debug, "Print parser trace");

    CLI11_PARSE(app, argc, argv);

    sinker::Context context;

    for (std::string const &input_filename : input_filenames)
    {
        std::ifstream file(input_filename, std::ios::binary);

        std::filesystem::path file_path(input_filename);
        sinker::Language language = file_path.extension() == ".skr" ? sinker::Language::SINKER : sinker::Language::SOURCE_CODE;

        if (!context.interpret(file, language, input_filename, debug))
            return 1;
    }

    if (!output_filename.empty())
    {
        std::ofstream output_stream(output_filename);
        context.dump(output_stream);
    }

    if (!def_filename.empty())
    {
        std::ofstream def_stream(def_filename);
        context.dump_def(def_stream);
    }

    return 0;
}
