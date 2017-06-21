#include "ImportedUgridFactory.h"
#include "mesh_analyzer.h"
#include "input.h"
#include <string>
#include <iostream>

void throwIfTrianglesHaveNegativeNodes(Input &input);
void throwIfQuadsHaveNegativeNodes(Input& input);
void throwIfTetsHaveNegativeNodes(Input &input);
void throwIfPyramidsHaveNegativeNodes(Input &input);
void throwIfPrismsHaveNegativeNodes(Input &input);
void throwIfHexsHaveNegativeNodes(Input &input);

int main(int argc,char* argv[]){
    Input input(argc,argv);
   
    if(input.isBigEndian())
        printf("Reading: %s as big endian\n",input.gridName().c_str());
    else
        printf("Reading: %s as little endian\n",input.gridName().c_str());

    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    Parfait::UgridReader::readHeader(input.gridName(),nnodes,ntri,nquad,ntet,npyr,nprism,nhex,input.isBigEndian());
    printf("Grid  has:\n");
    printf("---nodes     %i\n",nnodes);
    printf("---triangles      %i\n",ntri);
    printf("---quads          %i\n",nquad);
    printf("---tets           %i\n",ntet);
    printf("---pyramids       %i\n",npyr);
    printf("---prisms         %i\n",nprism);
    printf("---hexs           %i\n",nhex);

    throwIfTrianglesHaveNegativeNodes(input);
    throwIfQuadsHaveNegativeNodes(input);
    throwIfTetsHaveNegativeNodes(input);
    throwIfPyramidsHaveNegativeNodes(input);
    throwIfPrismsHaveNegativeNodes(input);
    throwIfHexsHaveNegativeNodes(input);

    return 0;
}

void throwIfTrianglesHaveNegativeNodes(Input &input) {
    printf("Checking triangles for negative node ids...\n");
    auto cells = Parfait::UgridReader::readTriangles(input.gridName(),input.isBigEndian());
    for(auto id:cells)
        if(id<0)
            throw std::domain_error("Triangles have negative nodes");
}

void throwIfQuadsHaveNegativeNodes(Input &input) {
    printf("Checking quads for negative node ids...\n");
    auto cells = Parfait::UgridReader::readQuads(input.gridName(),input.isBigEndian());
    for(auto id:cells)
        if(id<0)
            throw std::domain_error("Quads have negative nodes");
}


void throwIfTetsHaveNegativeNodes(Input &input) {
    printf("Checking tets for negative node ids...\n");
    auto cells = Parfait::UgridReader::readTets(input.gridName(),input.isBigEndian());
    for(auto id:cells)
        if(id<0)
            throw std::domain_error("Tets have negative nodes");
}

void throwIfPyramidsHaveNegativeNodes(Input &input) {
    printf("Checking pyramids for negative node ids...\n");
    auto cells = Parfait::UgridReader::readPyramids(input.gridName(),input.isBigEndian());
    for(auto id:cells)
        if(id<0)
            throw std::domain_error("Pyramids have negative nodes");
}

void throwIfPrismsHaveNegativeNodes(Input &input) {
    printf("Checking prisms for negative node ids...\n");
    auto cells = Parfait::UgridReader::readPrisms(input.gridName(),input.isBigEndian());
    for(auto id:cells)
        if(id<0)
            throw std::domain_error("Prisms have negative nodes");
}

void throwIfHexsHaveNegativeNodes(Input &input) {
    printf("Checking hexs for negative node ids...\n");
    auto cells = Parfait::UgridReader::readHexs(input.gridName(),input.isBigEndian());
    for(auto id:cells)
        if(id<0)
            throw std::domain_error("Hexs have negative nodes");
}
