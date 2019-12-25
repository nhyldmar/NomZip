//
// Created by Nicholas Hyldmar on 20/12/2019.
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
	
	template <typename SymbolType>
	ProbabilityModelGenerator<SymbolType>::ProbabilityModelGenerator() {
		this->setTemplateModel();
	}
	
	template <typename SymbolType>
	void ProbabilityModelGenerator<SymbolType>::setTemplateModel() {
		// TODO: Extend to variable length symbols.
		this->probabilityModel.clear();
		
		for (int symbol = 0; symbol <= 126; symbol++) {
			this->probabilityModel[symbol] = 0;
		}
	}
	
	template <typename SymbolType>
	void ProbabilityModelGenerator<SymbolType>::setBias(float &b) {
		this->bias = b;
	}
	
	template <typename SymbolType>
	void ProbabilityModelGenerator<SymbolType>::loadModel(std::string &filename) {
		this->probabilityModel.clear();
		
		std::ifstream file(filename);
		
		if (!file.is_open()) {
			std::cerr << "Unable to read from " << filename << std::endl;
		}
		
		SymbolType key;
		float value;
		
		size_t keySize = sizeof(key);
		size_t valueSize = sizeof(value);
		
		char *keyBuffer = new char[keySize];
		char *valueBuffer = new char[valueSize];
		
		while (file.tellg() < file.eof()) {
			file.read(keyBuffer, keySize);
			file.read(valueBuffer, valueSize);
			
			key = static_cast<SymbolType>(*keyBuffer);
			value = static_cast<float>(*valueBuffer);
			
			this->probabilityModel[key] = value;
		}
		
		std::cout << this->probabilityModel << std::endl;
	}
	
	template <typename SymbolType>
	void ProbabilityModelGenerator<SymbolType>::writeModel(std::string &filename) {
		// NOTE: Should probably put weight in the file
		std::ofstream file(filename);
		
		if (!file.is_open()) {
			std::cerr << "Unable to write to file " << filename << std::endl;
		}
		
		SymbolType key;
		float value;
		
		size_t keySize = sizeof(key);
		size_t valueSize = sizeof(value);
		
		for (auto &pair : this->probabilityModel) {
			key = pair.first;
			value = pair.second;
			
			file.write((char *) &key, keySize);
			file.write((char *) &value, valueSize);
		}
		
		std::cout << this->probabilityModel << std::endl;
	}
	
	template <typename SymbolType>
	std::map<SymbolType, float> ProbabilityModelGenerator<SymbolType>::getModel() {
		return this->probabilityModel;
	}
	
	template <typename SymbolType>
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
	template class ProbabilityModelGenerator<char>;
}
