//
// Created by Nicholas Hyldmar on 20/12/2019.
//

#include "ProbabilityModelGenerator.hpp"

#include <iostream>
#include <fstream>

namespace nz {
	int count(const std::string &string, const std::string &substring) {
		// TODO: Place this somewhere better
		// Credit: https://stackoverflow.com/a/22315816
		int count = 0;
		size_t nPos = string.find(substring, 0); // first occurrence
		while (nPos != std::string::npos) {
			count++;
			nPos = string.find(substring, nPos + 1);
		}
		
		return count;
	}
	
	ProbabilityModelGenerator::ProbabilityModelGenerator() {
		// TODO: Extend to multi-character symbols and non-UTF-8 characters.
		this->probabilityModel["\n"];
		
		for (int a = 32; a <= 126; a++) {
			std::string symbol(1, a);
			this->probabilityModel[symbol];
		}
	}
	
	void ProbabilityModelGenerator::setWeight(float w) {
		this->weight = w;
	}
	
	void ProbabilityModelGenerator::loadModel(std::string &filename) {
		std::ifstream file(filename);
		
		if (file.is_open()) {
			file >> this->weight;
			file >> this->probabilityModel;
		} else {
			std::cout << "Unable to read from " << filename << std::endl;
		}
	}
	
	void ProbabilityModelGenerator::writeModel(std::string &filename) {
		std::ofstream file(filename);
		
		if (file.is_open()) {
			file << this->weight;
			file << this->probabilityModel;
		}
	}
	
	nlohmann::json ProbabilityModelGenerator::getModel() {
		return this->probabilityModel;
	}
	
	void ProbabilityModelGenerator::processData(std::string &data) {
		// NOTE: Probably a better way of doing this
		float totalCount = 0;
		
		for (auto &entry : this->probabilityModel.items()) {
			float count = this->weight * static_cast<float>(entry.value());
			count += static_cast<float>(nz::count(data, entry.key()));
			
			totalCount += count;
			entry.value() = count;
		}
		
		for (auto &entry : this->probabilityModel.items()) {
			entry.value() = static_cast<float>(entry.value()) / totalCount;
		}
		
		this->weight = totalCount;
	}
}
