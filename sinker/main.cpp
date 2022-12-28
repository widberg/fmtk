#include <cstdlib>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <format>
#include <CLI/CLI.hpp>
#include <istream>

#include "sinker.hpp"

int main(int argc, char const* argv[]) {
    CLI::App app{"Sinker Compiler"};
    std::string output_filename = "a.skr";
    std::string def_filename;
    std::vector<std::string> input_filenames;
    bool debug = false;
    app.add_option("-o,--output", output_filename, "Output file");
    app.add_option("-d,--def", def_filename, "Definitions file");
    app.add_option("input_files", input_filenames, "Input files");
    app.add_flag("--debug,!--no-debug", debug, "Input files");

    CLI11_PARSE(app, argc, argv);

    Context context;
    
    for (std::string const& input_filename : input_filenames) {
        std::ifstream file(input_filename, std::ios::binary);
        
        std::filesystem::path file_path(input_filename);
        std::string file_extension(file_path.extension().string());

        Language language = file_extension == ".skr" ? Language::SINKER : Language::SOURCE_CODE;

        if (!context.interpret(file, language, input_filename, debug)) return 1;
    }

    std::ofstream output_stream(output_filename);
    context.dump(output_stream);

    if (!def_filename.empty()) {
        std::ofstream def_stream(def_filename);
        context.dump_def(def_stream);
    }

    return 0;
}
