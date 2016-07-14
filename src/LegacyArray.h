template<typename T>
class LegacyArray
{
	public:
		LegacyArray(T* arrayPointer_in,int arrayLength_in)
			:arrayPointer(arrayPointer_in),arrayLength(arrayLength_in){}
		int size(){return arrayLength;}
		T operator[](int i) const {return arrayPointer[i];}
		T &operator[](int i) {return arrayPointer[i];}
		class Iterator {
			public:
				Iterator(LegacyArray<T> &a,int index);
				void operator++();
				bool operator!=(const Iterator& rhs);
				T operator*();
				int operator-(const Iterator& subtrahend)
				{return index-subtrahend.index;}
			private:
				LegacyArray<T> &a;
				int index;
		};
		Iterator begin(){return Iterator(*this,0);}
		Iterator end()  {return Iterator(*this,this->size());}
		
	private:
		T* arrayPointer;
		int arrayLength;
};


template<typename T>
LegacyArray<T>::Iterator::Iterator(LegacyArray<T> &a_in,int index_in)
	:a(a_in),index(index_in)
{}

template<typename T>
void LegacyArray<T>::Iterator::operator++()
{++index;}

template<typename T>
bool LegacyArray<T>::Iterator::operator!=(const Iterator& rhs) {
	if(rhs.index != index)
		return true;
	return false;	
}


template<typename T>
T LegacyArray<T>::Iterator::operator*() {
	return a[index];
}