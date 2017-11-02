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
