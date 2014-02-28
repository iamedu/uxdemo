#include <ux/util.h>

std::string readFile(std::string filename) {
    std::ifstream infile(filename.data());
    std::stringstream buffer;

    buffer << infile.rdbuf();

    std::string result(buffer.str());

    return result;
}

