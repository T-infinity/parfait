
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
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
		return tag;
	return bcMap.find(tag)->second.first;
}

inline void Parfait::MapbcReader::print() {
	if(!failedToOpenMapbc){
		for(auto &item:bcMap){
			printf("Tag: %i Boundary Condition: %i\n",item.first,item.second.first);
		}
	}
}
