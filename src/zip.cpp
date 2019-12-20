//
// Created by Nicholas Hyldmar on 20/12/2019.
//

#include "cxxopts.hpp"

#include <iostream>
#include <string>

int main(int argc, char **argv) {
	// -------------------------------------------------------------------------------- Argument parsing
	cxxopts::Options options("NicZip", "zip <Input File> <Output File>");
	
	options.add_options()
			("in", "Input filename", cxxopts::value<std::string>())
			("out", "Output filename", cxxopts::value<std::string>()->default_value("out.nz"));
	
	auto result = options.parse(argc, argv);
	
	if (result.count("help")) {
		std::cout << options.help() << std::endl;
	}
	
	auto inputFilename = result["in"].as<std::string>();
	auto outputFilename = result["out"].as<std::string>();
	
	std::cout << "Input: " << inputFilename << " Output: " << outputFilename << std::endl;
	
	// -------------------------------------------------------------------------------- Load models
}
