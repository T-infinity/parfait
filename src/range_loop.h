#ifndef RANGE_LOOP_H
#define RANGE_LOOP_H
// This is a convenience tool for looping over ranges.
// There are several ways to use it.
//
// 1. Loop over a range of integers
// ---------------------------------------
// for(int i:range(5,9))
// 
// 2. Loop from zero to a number
// ---------------------------------------
// for(int i:range(6))
//
// 3. Loop over a range of an iteratable container
// ---------------------------------------
// for(int i:range(vec))
//
// 4. Loop over every nth element of an iteratable container
// ---------------------------------------
// for(int i:range(tets,4))
//
class range
{
	public:
		range(int low,int high):lo(low),hi(high){}
		range(int high):hi(high),lo(0){}
		template<typename T>
		inline range(T container):lo(0),hi(container.end()-container.begin()){}

		class Iterator
		{
			public:
				Iterator(range &myRange_in,int index_in):myRange(myRange_in),index(index_in){}
				void operator++() {++index;}
				bool operator!=(const Iterator& rhs) {return rhs.index!=index?true:false;}
				int operator*() {return index;}	
			private:
				range &myRange;
				int index;
		};
		Iterator begin() {return Iterator(*this,this->lo);}
		Iterator end()   {return Iterator(*this,this->hi);}

		int lo;
		int hi;
};


#endif
