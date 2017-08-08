#pragma once

inline void printHelpAndExit(){
    printf("\n\nPrinting help and Exiting\n");
    printf("Input arguments:\n");
    printf("   -i <input filename>\n");
    printf("      options: [lb8, b8, ascii]\n");
    printf("   -it <input file type>\n");
    printf("Output arguments:\n");
    printf("   -o <output filename>\n");
    printf("   -ot <output file type>\n");
    printf("      options: [vtk, vtk-surface, vtk-exploded, lb8 ]\n");
    exit(0);
}

inline void throwIfConfigIsInvalid(nlohmann::json config){
    std::string sdum = config["input"]["file"];
    sdum = config["input"]["type"];
    sdum = config["output"]["file"];
    sdum = config["output"]["type"];
}

inline void printConfig(nlohmann::json config){
    printf("Input grid: %s\n", config["input"]["file"]);
}

inline nlohmann::json getConfigFromCommandLine(std::vector<std::string>& command_line){
    nlohmann::json config;
    for(int index = 0; index < command_line.size(); index++){
        if(command_line[index] == "-h" or command_line[index] == "--help"){
            printHelpAndExit();
        }
        if(command_line[index] == "-i"){
            config["input"]["file"] = command_line[++index];
        } else if(command_line[index] == "-it"){
            config["input"]["type"] = command_line[++index];
        } else if(command_line[index] == "-o"){
            config["output"]["file"] = command_line[++index];
        } else if(command_line[index] == "-ot"){
            config["output"]["type"] = command_line[++index];
        }
    }
    try{
        throwIfConfigIsInvalid(config);
    } catch(std::exception &e){
        printf("Missing element in config");
        printHelpAndExit();
    }
    printConfig(config);
    return config;
}



