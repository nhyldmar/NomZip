//
// Created by Nicholas Hyldmar on 20/12/2019.
//

#ifndef NICZIP_PROBABILITYMODELGENERATOR_HPP
#define NICZIP_PROBABILITYMODELGENERATOR_HPP

#include "nlohmann/json.hpp"

#include <string>

namespace nz {
	class ProbabilityModelGenerator {
	public:
		ProbabilityModelGenerator();
		
		void setTemplateModel();
		
		void setWeight(float weight);
		
		void loadModel(std::string &filename);
		
		void writeModel(std::string &filename);
		
		void processData(std::string &data);
		
		nlohmann::json getModel();
	
	private:
		nlohmann::json probabilityModel = {};
		float weight = 0;
	};
}


#endif //NICZIP_PROBABILITYMODELGENERATOR_HPP
