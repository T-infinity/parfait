#ifndef MAPBC_READER_H
#define MAPBC_READER_H
#include <string>
#include <map>

class MapbcReader
{
	public: 
		MapbcReader(std::string filename);
		int boundaryCondition(int tag);
        std::map<int,int> getMap(){return bcMap;}
		void print();
	private:
		bool failedToOpenMapbc;
		std::string filename;
		std::map<int,int> bcMap;

};


#endif
