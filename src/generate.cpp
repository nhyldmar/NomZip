//
// Created by Nicholas Hyldmar on 20/12/2019.
//

#include "ProbabilityModelGenerator.hpp"

#include "cxxopts.hpp"

#include <string>
#include <vector>

#include <iostream>
#include <fstream>
#include <streambuf>
#include <memory>
#include <sys/stat.h>

// TODO: Find a better place to put this
// Credit: https://stackoverflow.com/a/12774387
inline bool fileExists(const std::string &name) {
	struct stat buffer{};
	return (stat(name.c_str(), &buffer) == 0);
}

int main(int argc, char **argv) {
	// -------------------------------------------------------------------------------- Argument parsing
	cxxopts::Options options("Generates a code table from <source file> and stores it in <output file>",
	                         "generate <source file> <model file>");
	
	options.add_options()
			("source", "Source filename", cxxopts::value<std::string>())
			("model", "Model filename", cxxopts::value<std::string>()->default_value("model.json"));
	
	auto result = options.parse(argc, argv);
	
	if (result.count("help")) {
		std::cout << options.help() << std::endl;
	}
	
	auto sourceFilename = result["source"].as<std::string>();
	auto modelFilename = result["model"].as<std::string>();
	
	// -------------------------------------------------------------------------------- Load content
	std::fstream sourceFile(sourceFilename);
	
	if (!sourceFile.is_open()) {
		std::cerr << "Unable to open " << sourceFilename;
		return 1;
	}
	
	// NOTE: This is probably a bad way of doing it, maybe better reading buffers of the maximum symbol length
	// Credit: https://stackoverflow.com/a/116220
	auto fileContent = std::string((std::istreambuf_iterator<char>(sourceFile)), std::istreambuf_iterator<char>());
	sourceFile.close();
	
	// -------------------------------------------------------------------------------- Generate Probability Model
	nz::ProbabilityModelGenerator probabilityModelGenerator;
	if (fileExists(modelFilename)) {
		probabilityModelGenerator.loadModel(modelFilename);
	}
	probabilityModelGenerator.processData(fileContent);
	probabilityModelGenerator.writeModel(modelFilename);
}
