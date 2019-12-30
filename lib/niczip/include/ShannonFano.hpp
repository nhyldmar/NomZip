//
// Created by Nicholas Hyldmar on 21/12/2019.
//

#ifndef NICZIP_SHANNONFANO_HPP
#define NICZIP_SHANNONFANO_HPP

#include "MapUtilities.hpp"

#include <string>
#include <map>
#include <cmath>
#include <bitset>

namespace nz {
	template<typename SymbolType>
	static std::map<SymbolType, char *> shannonFano(const std::map<SymbolType, float> &probabilityMap) {
		// NOTE: Probably good to erase maps after using them
		// Sort probabilities in descending order
		auto flippedProabilityMap = flip_map(probabilityMap); // Ascending order
		
		// Replace probabilities with cumulative probability up to that point
		std::map<SymbolType, std::pair<float, float>> cumulativeProbabilityMap;
		
		float totalProbability = 0;
		for (auto iter = flippedProabilityMap.rbegin(); iter != flippedProabilityMap.rend(); iter++) {
			auto symbol = iter->second;
			auto probability = iter->first;
			
			if (probability > 0) { // NOTE: Won't be able to encode symbols it hasn't seen before
				totalProbability += probability;
				cumulativeProbabilityMap.insert({symbol, {probability, totalProbability}});
			}
		}
		
		// NOTE: Inefficient memory usage with char*, better with bit array
		std::map<SymbolType, char *> codeTable;
		// For each symbol
		for (auto &pair : cumulativeProbabilityMap) {
			SymbolType symbol = pair.first;
			float probability = pair.second.first;
			float cumulativeProbability = pair.second.second;
			
			// Compute codeword length using ceil(-log_2(probability))
			int length = int(-std::log2(probability)) + 1;
			
			// NOTE: Should probably do better memory optimization
			char *codeword = new char[length];
			
			// For bit in codeword length
			for (int bit = length - 1; bit >= 0; bit--) {
				// Multiply associated cumulative probability by 2
				cumulativeProbability *= 2;
				
				// If cumulative probability >= 1, append 1 to codeword and subtract 1 from cumulative probability
				if (cumulativeProbability >= 1) {
					codeword[bit] = '1';
					cumulativeProbability -= 1;
				} else { // Else, append 0 to codeword
					codeword[bit] = '0';
				}
			}
			
			// Assign codeword in code table
			codeTable[symbol] = codeword;
		}
		
		// Return code table
		return codeTable;
	}
}


#endif //NICZIP_SHANNONFANO_HPP
