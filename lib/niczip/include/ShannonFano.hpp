//
// Created by Nicholas Hyldmar on 21/12/2019.
//

#ifndef NICZIP_SHANNONFANO_HPP
#define NICZIP_SHANNONFANO_HPP

#include "MapUtilities.hpp"

#include <string>
#include <map>
#include <algorithm>

namespace nz {
	template<typename SymbolType>
	static std::map<SymbolType, int> shannonFano(const std::map<SymbolType, float> &probabilityMap) {
		// NOTE: Might be good to erase maps after using them
		std::cout << probabilityMap << std::endl;
		
		auto flippedProabilityMap = flip_map(probabilityMap);
		
		std::cout << flippedProabilityMap << std::endl;
		
		// Replace probabilities with cumulative probability up to that point
		std::multimap<float, SymbolType> cumulativeProbabilityMap;
		
		float totalProbability = 0;
		for (auto iter = flippedProabilityMap.rbegin(); iter != flippedProabilityMap.rend(); iter--) {
			auto probability = iter->first;
			auto symbol = iter->second;
			
			totalProbability += probability;
			cumulativeProbabilityMap.insert({totalProbability, symbol});
		}
		
		std::cout << cumulativeProbabilityMap << std::endl;
		
		std::map<SymbolType, int> codeTable;
		// For each symbol
		// Compute codeword length using ceil(-log_2(probability))
		
		// For bit in codeword length
		// Multiply associated cumulative probability by 2
		
		// If cumulative probability >= 1, append 1 to codeword and subtract 1 from cumulative probability
		// Else, append 0 to codeword
		
		// Assign codeword in code table
		
		// Return code table
		return codeTable;
	}
}


#endif //NICZIP_SHANNONFANO_HPP
