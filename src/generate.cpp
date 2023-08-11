//
// Created by Naomi Hyldmar on 20/12/2019.
//

#include "ProbabilityModelGenerator.hpp"
#include "MapUtilities.hpp"

#include "cxxopts.hpp"

#include <string>
#include <iostream>
#include <fstream>
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
			("model", "Model filename", cxxopts::value<std::string>()->default_value("model.nzm"));

	auto result = options.parse(argc, argv);

	if (result.count("help")) {
		std::cout << options.help() << std::endl;
	}

	auto sourceFilename = result["source"].as<std::string>();
	auto modelFilename = result["model"].as<std::string>();

	// -------------------------------------------------------------------------------- Load content
	std::fstream sourceFile(sourceFilename);

	if (!sourceFile.is_open()) {
		std::cerr << "Unable to read from " << sourceFilename;
		return 1;
	}

	sourceFile.unsetf(std::ios::skipws);

	std::vector<char> byteVector((std::istream_iterator<char>(sourceFile)), std::istream_iterator<char>()); // Read file
	sourceFile.close();

	// -------------------------------------------------------------------------------- Generate Probability Model
	auto probabilityModelGenerator = std::make_shared<nz::ProbabilityModelGenerator<char>>();
	if (fileExists(modelFilename)) {
		probabilityModelGenerator->loadModel(modelFilename);
	} else {
		probabilityModelGenerator->setTemplateModel();
	}
	probabilityModelGenerator->processData(byteVector);
	probabilityModelGenerator->writeModel(modelFilename);
}
