#include "file_operations.h"
#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

bool confirmAction(const std::string& actionType) {
    std::string flag;
    std::cout << "Are you sure you want to proceed with the " << actionType << "? (Y/N): ";
    std::cin >> flag;

    if (flag == "N" || flag == "n") {
        std::cout << actionType << " canceled." << std::endl;
        return false;
    } else if (flag == "Y" || flag == "y") {
        return true;
    } else {
        std::cerr << "Unknown choice: " << flag << std::endl;
        return false;
    }
}

void createFile(const std::string& fileName) { 
    std::ofstream file(fileName);
    if (file) {
        std::cout << "File created successfully: " << fileName << std::endl;
    } else {
        std::cerr << "File creation failed!" << std::endl;
    }
}

bool createDir(const std::string& dirName) {
    try {
        fs::path dirPath = fs::current_path() / dirName;
        bool isDirCreated = fs::create_directory(dirPath);

        if (isDirCreated) {
            std::cout << "Directory created successfully: " << dirName << std::endl;
        } else {
            std::cerr << "Directory creation failed!" << std::endl;
        }

        return isDirCreated;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return false;
    }
}

void removeFile(const std::string& fileName) {
    if (!confirmAction("delete file")) {
        return;
    }
    try {
        if (fs::exists(fileName)) {
            if (fs::remove(fileName)) {
                std::cout << "File " << fileName << " deleted!" << std::endl;
            } else {
                std::cerr << "File deletion failed! (could not remove file)" << std::endl;
            }
        } else {
            std::cerr << "File deletion failed! (file does not exist)" << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
}

bool removeDir(const std::string& dirName) {
    if (!fs::exists(dirName)) {
        std::cout << "Directory doesn't exist: " << dirName << std::endl;
        return false;
    }
    if (!confirmAction("delete directory")) {
        return false;
    }
    try {
        bool isDirRemoved = fs::remove_all(dirName) > 0;
        if (isDirRemoved) {
            std::cout << "Directory removed successfully: " << dirName << std::endl;
        } else {
            std::cerr << "Directory removal failed!" << std::endl;
        }
        return isDirRemoved;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return false;
    }
}

void renameFileOrDir(const std::string& oldName, const std::string& newName) {
    if (!fs::exists(oldName)) {
        std::cout << "File or directory doesn't exist: " << oldName << std::endl;
        return;
    }

    try {
        fs::rename(oldName, newName);
        std::cout << "Renamed successfully to " << newName << std::endl;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming: " << e.what() << std::endl;
    }
}

void readFile(const std::string& fileName) {
    std::ifstream fileStream(fileName);
    
    if (!fileStream.is_open()) {
        std::cerr << "Error: Unable to open file " << fileName << std::endl;
        return;
    }

    std::string fileContent;
    while (std::getline(fileStream, fileContent)) {
        std::cout << fileContent << std::endl;
    }

    fileStream.close();
}

void writeFile(const std::string& fileName, const std::string& content) {
    std::ofstream fileStream(fileName);

    if (!fileStream.is_open()) {
        std::cerr << "Error: Unable to open file " << fileName << " for writing." << std::endl;
        return;
    }
    fileStream << content;
    fileStream.close();

    std::cout << "Content has been successfully written to file " << fileName << std::endl;
}
