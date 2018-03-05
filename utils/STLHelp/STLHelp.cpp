
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
#include <vector>
#include <STL.h>

int main(int argc, char* argv[]) {

    std::vector<std::string> command_line;
    for(int i = 0; i < argc; i++){
        command_line.push_back(argv[i]);
    }
    if(command_line.size() < 3){
        printf("Usage: to re-scale an stl file.\nArguments <input stl file> <scale factor>");
        exit(0);
    }

    Parfait::STL::STL stl;
    stl.loadBinaryFile(command_line[1]);
    double scale = atof(command_line[2].c_str());
    printf("rescaling %s by %lf\n", command_line[1].c_str(), scale);

    stl.rescale(scale);
    stl.writeAsciiFile(command_line[1] + ".rescaled");

    std::cout << std::endl;
    return 0;
}
