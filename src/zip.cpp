//
// Created by Nicholas Hyldmar on 20/12/2019.
//

#include "ProbabilityModelGenerator.hpp"
#include "ShannonFano.hpp"

#include "cxxopts.hpp"

#include <iostream>
#include <string>
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
	cxxopts::Options options("NicZip", "zip <Input File> <Output File> <Model File>");
	
	options.add_options()
			("in", "Input filename", cxxopts::value<std::string>())
			("out", "Output filename", cxxopts::value<std::string>()->default_value("out.nz"))
			("model", "Model filename", cxxopts::value<std::string>()->default_value("model.nzm"));
	
	auto result = options.parse(argc, argv);
	
	if (result.count("help")) {
		std::cout << options.help() << std::endl;
	}
	
	auto inputFilename = result["in"].as<std::string>();
	auto outputFilename = result["out"].as<std::string>();
	auto modelFilename = result["model"].as<std::string>();
	
	// -------------------------------------------------------------------------------- Load model
	auto probabilityModelGenerator = std::make_shared<nz::ProbabilityModelGenerator<char>>();
	if (fileExists(modelFilename)) {
		probabilityModelGenerator->loadModel(modelFilename);
	} else {
		std::cerr << "Unable to find " << modelFilename << std::endl;
		return 1;
	}
	auto probabilityModel = probabilityModelGenerator->getModel(); // NOTE: May be better memory management here
	
	// -------------------------------------------------------------------------------- Generate code table
	auto codeTable = nz::shannonFano(probabilityModel);
	std::cout << "Code table: " << codeTable << std::endl;
}
