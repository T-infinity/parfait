
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
#include "../../parfait/ImportedUgridFactory.h"
#include <iostream>
#include <sstream>
#include <memory>
#include <parfait/DistanceTree.h>
#include <parfait/STL.h>
#include <parfait/StringTools.h>
#include <parfait/ExtentWriter.h>

const std::string __VERSION = "0.2a";

std::vector<int> findAllTags(const Parfait::ImportedUgrid& mesh){
    std::set<int> tags;
    for(auto t : mesh.triangleTags)
        tags.insert(t);
    for(auto t : mesh.quadTags)
        tags.insert(t);
    return {tags.begin(), tags.end()};
}

void tellUserTags(const std::vector<int>& tags){
    std::cout << "There are these tags: ";
    for(auto t : tags) std::cout << " " << t;
    std::cout << std::endl;
}

int to_int(const std::string& s){
    std::stringstream buffer(s);
    int value;
    buffer >> value;
    return value;
}

std::set<int> getUserTags() {
    std::cout << "Which tags should be considered a wall? (space delimited list)" << std::endl;
    std::string user_tags;
    std::getline(std::cin, user_tags);
    auto l = Parfait::StringTools::split(user_tags, " ");
    std::set<int> tags;
    for(const auto& s : l){
        tags.insert(to_int(s));
    }
    return tags;
}

std::set<int> getWallTags(const Parfait::ImportedUgrid& mesh){
    auto all_tags = findAllTags(mesh);
    tellUserTags(all_tags);
    auto tags = getUserTags();
    std::cout << "Measuring distance to nearest tag: ";
    for(auto t : tags){
        std::cout << " " << t;
    }
    std::cout << std::endl;
    return tags;
}

std::vector<int> determineIfBoundaryNodes(const Parfait::ImportedUgrid& ugrid){
    int nnodes = ugrid.nodes.size()/3;
    std::vector<int> is_boundary(nnodes, 0);
    for(int t = 0; t < ugrid.triangles.size() /3; t++){
        for(int i = 0; i < 3; i++){
            int n = ugrid.triangles[3*t+i];
            is_boundary[n] = 1;
        }
    }
    for(int q = 0; q < ugrid.quads.size() /4; q++){
        for(int i = 0; i < 4; i++){
            int n = ugrid.quads[4*q+i];
            is_boundary[n] = 1;
        }
    }
    return is_boundary;
}

auto cacheSurface(const Parfait::ImportedUgrid& ugrid, const std::set<int>& tags) {
    Parfait::STL::STL stl;
    for(int t = 0; t < ugrid.triangles.size() / 3; t++) {
        auto tri = &ugrid.triangles[3 * t + 0];
        int tag = ugrid.triangleTags[t];
        if (tags.count(tag) != 0) {
            std::array<Parfait::Point<double>, 3> facet;
            for (int i = 0; i < 3; i++) {
                int n = tri[i];
                const double *p = &ugrid.nodes[3 * n + 0];
                facet[i] = Parfait::Point<double>(p[0], p[1], p[2]);
            }
            stl.facets.push_back({facet[0], facet[1], facet[2]});
        }
    }

    for(int q = 0; q < ugrid.quads.size() / 4; q++) {
        auto quad = &ugrid.quads[4 * q + 0];
        int tag = ugrid.quadTags[q];
        if (tags.count(tag) != 0) {
            std::array<Parfait::Point<double>, 3> facet;
            const double *p;
            p = &ugrid.nodes[3 * quad[0] + 0];
            facet[0] = Parfait::Point<double>(p[0], p[1], p[2]);
            p = &ugrid.nodes[3 * quad[1] + 0];
            facet[1] = Parfait::Point<double>(p[0], p[1], p[2]);
            p = &ugrid.nodes[3 * quad[2] + 0];
            facet[2] = Parfait::Point<double>(p[0], p[1], p[2]);
            stl.facets.push_back({facet[0], facet[1], facet[2]});

            p = &ugrid.nodes[3 * quad[2] + 0];
            facet[2] = Parfait::Point<double>(p[0], p[1], p[2]);
            p = &ugrid.nodes[3 * quad[3] + 0];
            facet[3] = Parfait::Point<double>(p[0], p[1], p[2]);
            p = &ugrid.nodes[3 * quad[0] + 0];
            facet[0] = Parfait::Point<double>(p[0], p[1], p[2]);
            stl.facets.push_back({facet[0], facet[1], facet[2]});
        }
    }
    return stl;
}

void writeToFile(std::string filename, const std::vector<double>& dist, const std::vector<double>& xyz){
    FILE* fp = fopen(filename.c_str(), "w");

    fprintf(fp, "#nodeId: x y z distance\n");
    for(int n = 0; n < dist.size(); n++){
        fprintf(fp,"%d: %1.15e %1.15e %1.15e %1.15e\n", n, xyz[3*n+0], xyz[3*n+1], xyz[3*n+2], dist[n]);
    }

    fclose(fp);
}

void writeBoundaryDistances(std::string filename, const std::vector<double>& dist, const std::vector<int>& is_boundary){
    int nnodes = is_boundary.size();
    FILE* fp = fopen(filename.c_str(), "w");

    for(int n = 0; n < nnodes; n++){
        if(is_boundary[n]){
            fprintf(fp,"%d: %e\n", n, dist[n]);
        }
    }
    fclose(fp);
}

void printVersionAndExit(){
    std::cout << __VERSION << std::endl;
    exit(0);
}

bool isVersionRequested(const std::vector<std::string>& arguments){
    for(const auto& a : arguments){
        if(a == "-v" or a == "--version")
            return true;
    }
    return false;
}

bool isAppropriateFilenameRequested(const std::vector<std::string>& arguments){
    if(arguments.size() == 1)
        return false;
    auto should_be_filename = arguments[1];
    auto filename_parts = Parfait::StringTools::split(should_be_filename, ".");
    return filename_parts.back() == "ugrid";
}

void printHelpAndExit(){
    std::cout << "DistanceCalculator" << std::endl;
    std::cout << "Usage: ./DistanceCalculator <filename>" << std::endl;
    std::cout << "Only *.b8.ugrid or *.lb8.ugrid files are supported at this time.";
    exit(0);
}

bool isBigEndian(std::string filename) {
    return filename.find(".b8.") != std::string::npos;
}

std::vector<std::string> launderArguments(int argc, char* argv[]){
    std::vector<std::string> arguments;
    for(int i = 0; i < argc; i++){
        arguments.push_back(argv[i]);
    }
    return arguments;
}

std::string getFilename(const std::vector<std::string>& arguments){
    if(not isAppropriateFilenameRequested(arguments))
        printHelpAndExit();
    return arguments[1];
}
int main(int argc, char* argv[]) {
    std::vector<std::string> arguments = launderArguments(argc, argv);
    if(isVersionRequested(arguments))
        printVersionAndExit();

    std::string filename = getFilename(arguments);
    bool is_big_endian = isBigEndian(filename);

    auto ugrid = Parfait::ImportedUgridFactory::readUgrid(filename, is_big_endian);
    auto tags = getWallTags(ugrid);


    auto stl = cacheSurface(ugrid, tags);

    Parfait::DistanceTree tree(stl.findDomain());
    tree.setMaxDepth(26);

    for(const auto& f : stl.facets){
        tree.insert(f);
    }
    tree.finalize();

    auto start = std::chrono::system_clock::now();
    int nnodes = ugrid.nodes.size() / 3;
    std::vector<double> dist(nnodes, -1);
    printf("Searching for distance of %d points\n", nnodes);
#pragma omp parallel for schedule(guided)
    for(int n = 0; n < nnodes; n++){
        auto p = &ugrid.nodes[3*n+0];
        Parfait::Point<double> point(p[0], p[1], p[2]);
        auto c = tree.closestPoint(p);
        dist[n] = (point - c).magnitude();
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Elapsed time " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

    writeToFile("distance.txt", dist, ugrid.nodes);
    writeBoundaryDistances("boundary.txt", dist, determineIfBoundaryNodes(ugrid));
    //printf("Exporting <%s> as vtu.", filename.c_str());
    //Parfait::VtkUnstructuredWriter writer(filename, ugrid);
    //writer.addNodeData("distance", dist.data(), 1);
    //writer.writeBinary();
}
