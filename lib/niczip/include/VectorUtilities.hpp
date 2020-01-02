//
// Created by Nicholas Hyldmar on 31/12/2019.
//

#ifndef NICZIP_VECTORUTILITIES_HPP
#define NICZIP_VECTORUTILITIES_HPP

#include <vector>

template<typename Type>
std::ostream &operator<<(std::ostream &ostream, const std::vector<Type> &vector) {
	ostream << '[';
	
	for (auto iterator = vector.cbegin(); iterator != vector.cend(); iterator++) {
		if (iterator != vector.cbegin()) {
			ostream << ',';
		}
		
		ostream << *iterator;
	}
	
	ostream << ']';
	
	return ostream;
}

#endif //NICZIP_VECTORUTILITIES_HPP
