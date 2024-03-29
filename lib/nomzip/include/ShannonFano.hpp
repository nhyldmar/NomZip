//
// Created by Naomi Hyldmar on 21/12/2019.
//

#ifndef NOMZIP_SHANNONFANO_HPP
#define NOMZIP_SHANNONFANO_HPP

#include "MapUtilities.hpp"

#include <vector>
#include <map>
#include <cmath>

namespace nz {
	template<typename SymbolType>
	static std::map<SymbolType, std::vector<char>> shannonFano(const std::map<SymbolType, float> &probabilityMap) {
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

		// NOTE: Might be better to replace vector with something else
		std::map<SymbolType, std::vector<char>> codeTable;
		// For each symbol
		for (auto &pair : cumulativeProbabilityMap) {
			SymbolType symbol = pair.first;
			float probability = pair.second.first;
			float cumulativeProbability = pair.second.second;

			// Compute codeword length using ceil(-log_2(probability))
			int length = int(-std::log2(probability)) + 1;

			// For bit in codeword length
			std::vector<char> codeword;
			for (int bit = length - 1; bit >= 0; bit--) {
				// Multiply associated cumulative probability by 2
				cumulativeProbability *= 2;

				// If cumulative probability >= 1, append 1 to codeword and subtract 1 from cumulative probability
				if (cumulativeProbability >= 1) {
					codeword.push_back(1);
					cumulativeProbability -= 1;
				} else { // Else, append 0 to codeword
					codeword.push_back(0);
				}
			}

			// Assign codeword in code table
			codeTable[symbol] = codeword;
		}

		// Return code table
		return codeTable;
	}
}


#endif //NOMZIP_SHANNONFANO_HPP
