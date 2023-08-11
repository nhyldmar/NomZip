//
// Created by Naomi Hyldmar on 20/12/2019.
//

#include "ProbabilityModelGenerator.hpp"

#include "MapUtilities.hpp"

#include <sys/stat.h>
#include <iostream>
#include <fstream>

namespace nz {
	inline bool fileExists(const std::string &name) {
		// Credit: https://stackoverflow.com/a/12774387
		struct stat buffer{};
		return (stat(name.c_str(), &buffer) == 0);
	}

	int count(const std::string &string, const std::string &substring) {
		// Credit: https://stackoverflow.com/a/22315816
		int count = 0;
		size_t nPos = string.find(substring, 0); // first occurrence
		while (nPos != std::string::npos) {
			count++;
			nPos = string.find(substring, nPos + 1);
		}

		return count;
	}

	template<typename SymbolType>
	ProbabilityModelGenerator<SymbolType>::ProbabilityModelGenerator() = default;

	template<typename SymbolType>
	void ProbabilityModelGenerator<SymbolType>::setTemplateModel() {
		// TODO: Extend to variable length symbols.
		this->probabilityModel.clear();

		for (int symbol = 0; symbol <= 127; symbol++) {
			this->probabilityModel[symbol] = 0;
		}
	}

	template<typename SymbolType>
	void ProbabilityModelGenerator<SymbolType>::setBias(float &b) {
		this->bias = b;
	}

	template<typename SymbolType>
	void ProbabilityModelGenerator<SymbolType>::loadModel(std::string &filename) {
		this->probabilityModel.clear();

		std::ifstream file(filename);

		if (!file.is_open()) {
			std::cerr << "Unable to read from " << filename << std::endl;
		}

		while (file.peek() != EOF) {
			SymbolType key;
			float value;

			file.read((char *) &key, sizeof(key));
			file.read((char *) &value, sizeof(value));

			this->probabilityModel[key] = value;
		}
	}

	template<typename SymbolType>
	void ProbabilityModelGenerator<SymbolType>::writeModel(std::string &filename) {
		// NOTE: Should probably put weight in the file
		std::ofstream file(filename);

		if (!file.is_open()) {
			std::cerr << "Unable to write to file " << filename << std::endl;
		}

		for (std::pair<SymbolType, float> pair : this->probabilityModel) {
			// NOTE: auto will actually assign type int to a char which is 4 bytes instead of 1
			auto key = pair.first;
			auto value = pair.second;

			file.write((char *) &key, sizeof(key));
			file.write((char *) &value, sizeof(value));
		}
	}

	template<typename SymbolType>
	std::map<SymbolType, float> ProbabilityModelGenerator<SymbolType>::getModel() {
		return this->probabilityModel;
	}

	template<typename SymbolType>
	void ProbabilityModelGenerator<SymbolType>::processData(std::vector<SymbolType> &data) {
		// NOTE: Probably a better way of doing this
		float totalCount = 0;

		for (auto &pair : this->probabilityModel) {
			auto symbol = pair.first;
			auto probability = pair.second;

			float count = this->bias * probability;
			// NOTE: Probably doesn't work for SymbolType != char
			count += std::count(data.begin(), data.end(), symbol);

			totalCount += count;
			pair.second = count;
		}

		for (auto &pair : this->probabilityModel) {
			pair.second /= totalCount;
		}

		this->bias = totalCount;
	}

	// Linker, why must you be this way
	template
	class ProbabilityModelGenerator<char>;
}
