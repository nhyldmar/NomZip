//
// Created by Nicholas Hyldmar on 20/12/2019.
//

#include "ProbabilityModelGenerator.hpp"
#include "ShannonFano.hpp"

#include "cxxopts.hpp"

#include <iostream>
#include <fstream>
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
	// TODO: Auto-generate model if none specified
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
	
	// -------------------------------------------------------------------------------- Load input file
	std::fstream originalFile(inputFilename);
	
	if (!originalFile.is_open()) {
		std::cerr << "Unable to read from " << inputFilename;
		return 1;
	}
	
	originalFile.unsetf(std::ios::skipws);
	
	std::vector<char> byteVector((std::istream_iterator<char>(originalFile)), std::istream_iterator<char>()); // Read file
	originalFile.close();
	
	// -------------------------------------------------------------------------------- Encode file
	// TODO: Move file encoding into a class
	
	std::vector<char> encodedBitVector;
	for (auto &symbol : byteVector) {
		auto codeword = codeTable[symbol];
		encodedBitVector.insert(encodedBitVector.end(), codeword.begin(), codeword.end());
	}
	
	// NOTE: This should be made portable by using CHAR_BIT from <type_traits>
	char paddingSize = static_cast<char>((8 - encodedBitVector.size() % 8) % 8);
	std::vector<char> padding(3 + paddingSize, 0); // The 3 is because paddingSize <= 8, so can be stored in 3 bits
	for (int i = 0; i <= 2; i++) {
		padding[2 - i] = paddingSize & 1;
		paddingSize >>= 1;
	}
	
	encodedBitVector.insert(encodedBitVector.begin(), padding.begin(), padding.end());
	
	if (encodedBitVector.size() % 8 != 0) {
		std::cerr << "For some reason the padding didn't work. Please report this issue." << std::endl;
	}
	
	std::vector<char> encodedByteVector;
	char byte = 0;
	for (int bitIndex = 0; bitIndex < encodedBitVector.size();) {
		auto bit = encodedBitVector[bitIndex];
		byte = static_cast<char>(byte * 2 + bit);
		bitIndex++;
		
		if (bitIndex % 8 == 0) {
			encodedByteVector.push_back(byte);
			byte = 0;
		}
	}
	
	// -------------------------------------------------------------------------------- Write encoded file
	std::ofstream compressedFile(outputFilename);
	
	if (!compressedFile.is_open()) {
		std::cerr << "Unable to write to " << outputFilename;
		return 1;
	}
	
	std::copy(encodedByteVector.begin(), encodedByteVector.end(), std::ostream_iterator<char>(compressedFile));
	originalFile.close();
	
	// -------------------------------------------------------------------------------- Benchmark compression
	size_t inSize = byteVector.size();
	size_t outSize = encodedByteVector.size();
	float compressionRatio = (float) inSize / (float) outSize;
	std::cout << "Compression ratio: " << compressionRatio << std::endl;
}
