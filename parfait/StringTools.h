#include <string>
#include <cstring>

namespace StringTools {

    inline std::vector<std::string> split(const std::string& s, const std::string& delimiter){
        std::vector<std::string> elements;
        auto src = s.c_str();
        const char* next = src;
        std::string out;
        while ((next = strstr(src, delimiter.c_str())) != NULL) {
            while (src != next)
                out += *src++;
            elements.push_back(out);
            src += delimiter.size();
            out = "";
        }

        while (*src != '\0')
            out += *src++;
        elements.push_back(out);
        return elements;
    }
}
