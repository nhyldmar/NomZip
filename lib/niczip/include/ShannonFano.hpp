//
// Created by Nicholas Hyldmar on 21/12/2019.
//

#ifndef NICZIP_SHANNONFANO_HPP
#define NICZIP_SHANNONFANO_HPP

#include "ExtendedTree.hpp"

#include "nlohmann/json.hpp"

#include <string>
#include <map>
#include <algorithm>

namespace nz {
	// Credit: https://stackoverflow.com/a/5056797
	template<typename A, typename B>
	std::pair<B, A> flip_pair(const std::pair<A, B> &p) {
		return std::pair<B, A>(p.second, p.first);
	}
	
	template<typename A, typename B>
	std::multimap<B, A> flip_map(const std::map<A, B> &src) {
		std::multimap<B, A> dst;
		std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()), flip_pair<A, B>);
		return dst;
	}
	
	static nlohmann::json shannonFano(nlohmann::json &probabilityModel) {
		// Sort model by value in decreasing order
		
		// Replace probabilities with cumulative probability up to that point
		
		// For each symbol
			// Compute codeword length using ceil(-log_2(probability))
			
			// For bit in codeword length
				// Multiply associated cumulative probability by 2
				
				// If cumulative probability >= 1, append 1 to codeword and subtract 1 from cumulative probability
				// Else, append 0 to codeword
				
			// Assign codeword in code table
			
		// Return code table
	}
}


#endif //NICZIP_SHANNONFANO_HPP
