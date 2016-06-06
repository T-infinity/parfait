#ifndef UTIL_INPUT_H
#define UTIL_INPUT_H
#include <iostream>
#include <string>

using namespace std;
class Input{
    public:
        Input(int argc,char* argv[]){
            setRawInput(argc,argv);
            is_big_endian = deduceIsBigEndian();
            setPathAndProject();
        }
        string gridName(){return path+project+".ugrid";}
        string projectName(){return project;}
        bool isBigEndian(){return is_big_endian;}
    private:
        bool is_big_endian=false;
        string path;
        string project;
        string raw_input;

        void setRawInput(int argc,char* argv[]){
            if(2 == argc)
                raw_input = argv[1];
            else{
                printf("Enter grid name: ");
                cin >> raw_input;
            }

        }

        bool deduceIsBigEndian(){
            if(-1 != int(raw_input.find(".lb8.")))
                return false;
            if(-1 != int(raw_input.find(".b8.")))
                return true;
            while(true){
                printf("Enter endianness (b/l): ");
                char c;
                cin >> c;
                if('b' == c) return true;
                if('l' == c) return false;
            }
        }
        
        void setPathAndProject(){
            auto position = raw_input.find(".ugrid");
            project = raw_input.substr(0,position);
            path = project;
            project = stripPath(project);
            position = path.find_last_of("/");
            if(-1 == int(position))
                path = "";
            else
                path = path.substr(0,position+1);
        }

        string stripPath(string s){
            return s.substr(s.find_last_of("/")+1);
        }
};

#endif
