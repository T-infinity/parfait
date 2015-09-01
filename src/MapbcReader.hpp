#include <stdio.h>
#include <assert.h>
#include <string>

inline Parfait::MapbcReader::MapbcReader(std::string filename_in)
	:filename(filename_in)
{
	FILE *f = fopen(filename.c_str(),"r");
	if(f == NULL) {
		failedToOpenMapbc = true;
		fprintf(stderr,"Warning: could not open %s",filename.c_str());
		fprintf(stderr," setting boundary conditions to zero\n");
	}
	else {
		failedToOpenMapbc = false;
		int numberOfTags = 0;
		fscanf(f,"%i",&numberOfTags);
		for(int i=0;i<numberOfTags;i++) {
			int tag,boundaryCondition;
			char name[80];
			fscanf(f,"%i %i %s",&tag,&boundaryCondition,name);
			bcMap[tag] = std::make_pair(boundaryCondition,std::string(name));
		}

		fclose(f);
	}
}

inline int Parfait::MapbcReader::boundaryCondition(int tag) {
	if(failedToOpenMapbc)
		return 0;
	return bcMap.find(tag)->second.first;
}

inline void Parfait::MapbcReader::print() {
	if(!failedToOpenMapbc){
		for(auto &item:bcMap){
			printf("Tag: %i Boundary Condition: %i\n",item.first,item.second.first);
		}
	}
}
