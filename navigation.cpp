#include "navigation.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void displayCurrentDirectory() { 
    fs::path currentPath = fs::current_path();
    std::cout << "Current Dir: " << currentPath << std::endl;
}

void listDirContents() { 
    for (const auto& entry : fs::directory_iterator(fs::current_path())) { 
        std::cout << (entry.is_directory()? "[DIR] " : "[FILE] ") << entry.path().filename().string() << std::endl;
    }
}

void changeDir(const std::string& newPath) { 
    fs::path targetPath = newPath;
    if (fs::exists(targetPath) && fs::is_directory(targetPath)) {
        fs::current_path(targetPath);
        std::cout << "Changed Directory to: " << fs::current_path() << std::endl;
    } else {
        std::cerr << "Invalid Directory: " << newPath << std::endl;
    }
}

void goToParentDirectory() { 
    fs::path parentPath = fs::current_path().parent_path();
    if (!parentPath.empty()) { 
        fs::current_path(parentPath);
        std::cout << "Changed to parent directory: " << fs::current_path() << std::endl;
    } else {
        std::cerr << "Already at the root directory" << std::endl;
    }
}

