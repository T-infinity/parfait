#pragma once
#include <vector>

namespace Parfait {
	template<class Container, class T>
	inline bool isIn(const Container &container, T t) {
		for (auto u : container) {
			if (t == u)
				return true;
		}
		return false;
	}

	template<typename T>
	void insertUnique(std::vector<T> &vec, T n) {
		for (auto it = vec.begin(); it < vec.end(); it++) {
			if (n == *it)  // don't insert duplicate
				return;
			if (n < *it)   // insert in front of next biggest element
			{
				vec.insert(it, n);
				return;
			}
		}
		if (0 == (int) vec.size()) // if empty, push back
			vec.push_back(n);
		else if (vec.back() < n)  // if all elements smaller, push back
			vec.push_back(n);
	}
}