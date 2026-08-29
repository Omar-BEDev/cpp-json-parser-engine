#include <fstream>
#include <iostream>
#include <sstream>

std::string extract_file_content(std::string path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "Not found file";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}