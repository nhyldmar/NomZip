//
// Created by Nicholas Hyldmar on 20/12/2019.
//

#ifndef NICZIP_PROBABILITYMODELGENERATOR_HPP
#define NICZIP_PROBABILITYMODELGENERATOR_HPP

#include "nlohmann/json.hpp"

#include <string>
#include <vector>
#include <map>

namespace nz {
	template <typename SymbolType>
	class ProbabilityModelGenerator {
	public:
		ProbabilityModelGenerator();
		
		void setTemplateModel();
		
		void setBias(float &b);
		
		void loadModel(std::string &filename);
		
		void writeModel(std::string &filename);
		
		void processData(std::vector<SymbolType> &data);
		
		std::map<SymbolType, float> getModel();
	
	private:
		std::map<SymbolType, float> probabilityModel = {};
		float bias = 0;
	};
}


#endif //NICZIP_PROBABILITYMODELGENERATOR_HPP
