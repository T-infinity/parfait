#include "mapbc_reader.h"
#include <stdio.h>
#include <assert.h>
#include <string>

MapbcReader::MapbcReader(std::string filename_in)
	:filename(filename_in)
{
	FILE *f = fopen(filename.c_str(),"r");
	if(f == NULL)
	{
		failedToOpenMapbc = true;
		fprintf(stderr,"Warning: could not open %s",filename.c_str());
		fprintf(stderr," setting boundary conditions to zero\n");
	}
	else
	{
		failedToOpenMapbc = false;
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
}


int MapbcReader::boundaryCondition(int tag)
{
	if(failedToOpenMapbc)
		return 0;
	return bcMap.find(tag)->second;
}

void MapbcReader::print()
{
	if(!failedToOpenMapbc){
		for(auto &item:bcMap){
			printf("Tag: %i Boundary Condition: %i\n",item.first,item.second);
		}
	}
}
