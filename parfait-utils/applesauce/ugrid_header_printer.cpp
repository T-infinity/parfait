#include <stdexcept>
#include <iostream>
#include <string>
#include "UgridReader.h"

int main(int argc,char* argv[]){
	
    if(argc != 2) {
		throw std::logic_error("name of grid required as input");
	}
	std::string grid_filename = argv[1];
	printf("Endianness (b/l):");
	char endianness;
	std::cin >> endianness;
	bool is_big_endian = false;
	if(endianness == 'b' or endianness == 'B')
		is_big_endian = true;
	int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
	Parfait::UgridReader::readHeader(grid_filename, nnodes,
			ntri,nquad,ntet,npyr,nprism,nhex,is_big_endian);
	printf("Grid name: %s\n",grid_filename.c_str());
	printf("Number of nodes:      %i\n",nnodes);
	printf("Number of triangles:  %i\n",ntri);
	printf("Number of quads:      %i\n",nquad);
	printf("Number of tets:       %i\n",ntet);
	printf("Number of pyramids:   %i\n",npyr);
	printf("Number of prisms:     %i\n",nprism);
	printf("Number of hexs:       %i\n",nhex);
	return 0;
}
