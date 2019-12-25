//
// Created by Nicholas Hyldmar on 21/12/2019.
//

#ifndef NICZIP_MAPUTILITIES_HPP
#define NICZIP_MAPUTILITIES_HPP

#include <string>
#include <vector>
#include <iostream>

template<typename KeyType, typename ValueType>
std::ostream &operator<<(std::ostream &ostream, const std::map<KeyType, ValueType> &map) {
	for (auto &pair : map) {
		ostream << pair.first << ' ' << pair.second << '\n';
	}
	
	return ostream;
}

template<typename KeyType, typename ValueType>
std::ostream &operator<<(std::ostream &ostream, const std::multimap<KeyType, ValueType> &map) {
	for (auto &pair : map) {
		ostream << pair.first << ' ' << pair.second << '\n';
	}
	
	return ostream;
}

template<typename KeyType, typename ValueType>
std::istream &operator>>(std::istream &istream, std::map<KeyType, ValueType> &map) {
	KeyType key;
	ValueType value;
	
	while (istream >> key >> value) {
		map[key] = value;
	}
	
	return istream;
}

template<typename KeyType, typename ValueType>
std::istream &operator>>(std::istream &istream, std::multimap<KeyType, ValueType> &map) {
	KeyType key;
	ValueType value;
	
	while (istream >> key >> value) {
		map[key] = value;
	}
	
	return istream;
}

// Credit: https://stackoverflow.com/a/5056797
template<typename A, typename B>
std::pair<B, A> flip_pair(const std::pair<A, B> &p) {
	return std::pair<B, A>(p.second, p.first);
}

template<typename A, typename B, template<class, class, class...> class M, class... Args>
std::multimap<B, A> flip_map(const M<A, B, Args...> &src) {
	std::multimap<B, A> dst;
	std::transform(src.begin(), src.end(),
	               std::inserter(dst, dst.begin()),
	               flip_pair<A, B>);
	return dst;
}

#endif //NICZIP_MAPUTILITIES_HPP
