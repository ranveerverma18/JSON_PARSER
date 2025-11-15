#include "../include/FileUtils.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::string FileUtils::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + path);

    std::stringstream buffer;  //create a stringstream object to hold the file contents
    buffer << file.rdbuf();    //dumps the entire file content into the stringstream
    return buffer.str();       //return the content as a string
}

void FileUtils::writeFile(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Cannot write to file: " + path);

    file << content;
}
