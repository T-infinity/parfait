#ifndef VECTOR_TOOLS_H
#define VECTOR_TOOLS_H
#include <vector>

template <class Container, class T>
inline bool isIn(const Container & container, T t){
    for(auto u : container){
        if(t == u)
            return true;
    }
    return false;
}

// Searches Linearly if an item is in a vector
// Can be used on non-sorted arrays (and non-sortable data structures)
template <class T>
inline bool isIn(const std::vector<T> &vec, T t){

    for(auto u : vec){
        if(t == u){
            return true;
        }
    }
    return false;
}

// Insert an item into a vector if that item 
// is not already in the vector.
// Cannot be used on data that doesn't not have the < operator overloaded.
template<typename T>
void insertUnique(std::vector<T> &vec,T n)
{
	for(auto it=vec.begin();it<vec.end();it++)
	{
		if(n == *it)  // don't insert duplicate
			return;
		if(n < *it)   // insert in front of next biggest element
		{
			vec.insert(it,n);
			return;
		}
	}
	if(0 == (int)vec.size()) // if empty, push back
		vec.push_back(n);
	else if(vec.back() < n)  // if all elements smaller, push back
		vec.push_back(n);
}


#endif
