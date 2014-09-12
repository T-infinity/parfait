#include "mapbc_reader.h"
#include <stdio.h>
#include <assert.h>

MapbcReader::MapbcReader(std::string filename_in)
	:filename(filename_in)
{
	FILE *f = fopen(filename.c_str(),"r");
	assert(f != NULL);
	int numberOfTags = 0;
	fscanf(f,"%i",&numberOfTags);
	// put tag, boundary-condition pairs in a map
	for(int i=0;i<numberOfTags;i++)
	{
		int tag,boundaryCondition;
		char name[80];
		fscanf(f,"%i %i %s",&tag,&boundaryCondition,name);
		bcMap.insert(std::pair<int,int>(tag,boundaryCondition));
	}

	fclose(f);
}

int MapbcReader::boundaryCondition(int tag)
{
	return bcMap.find(tag)->second;
}

void MapbcReader::print()
{
	for(auto &item:bcMap)
	{
		printf("Tag: %i Boundary Condition: %i\n",item.first,item.second);
	}
}
