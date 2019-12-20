//
// Created by Nicholas Hyldmar on 20/12/2019.
//

#ifndef NICZIP_NZTYPES_H
#define NICZIP_NZTYPES_H

#include <string>
#include <vector>
#include <map>

namespace nz {
	struct ProbabilityMap {
		std::map<std::string, float> map;
		float weight;
	};
	
	struct ExtendedNode {
	
	};
	
	typedef std::vector<ExtendedNode> ExtendedTree;
}

#endif //NICZIP_NZTYPES_H
