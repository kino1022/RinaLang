#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

inline std::string read_entire_file (const std::string& path) {
    std::ifstream ifs (path, std::ios::binary);
    if (!ifs) throw std::runtime_error("Failed to open file: " + path);
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return oss.str();
}

