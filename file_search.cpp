#include "file_search.h"
#include <iostream>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;

std::vector<std::string> searchFiles(const std::string& searchTerm) {
    std::vector<std::string> foundItems;
    fs::path startDir = fs::current_path();
    for (const auto& entry : fs::recursive_directory_iterator(startDir)) {
        if (entry.is_regular_file() && entry.path().filename().string().find(searchTerm) != std::string::npos) {
            foundItems.push_back(entry.path().string());
        }
        if (entry.is_directory() && entry.path().filename().string().find(searchTerm) != std::string::npos) {
            foundItems.push_back(entry.path().string());
        }
    }
    return foundItems;
}

std::vector<std::string> searchByExtension(const std::string& extension) {
    std::vector<std::string> foundItems;
    fs::path startDir = fs::current_path();
    for (const auto& entry : fs::recursive_directory_iterator(startDir)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            foundItems.push_back(entry.path().string());
        }
    }
    return foundItems;
}
