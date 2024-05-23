#include "navigation.h"
#include "file_operations.h"
#include "file_search.h"
#include "file_sort.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>


void handleUserInput() {
    std::string command;
    while (true) {
        std::cout << "Choose an option:\n"
                  << "1. Navigation commands (ls, cd <dir>, cd ..)\n"
                  << "2. Create files or directories\n"
                  << "3. Delete files or directories\n"
                  << "4. Rename files or directories\n"
                  << "5. Read or write files\n"
                  << "6. Search files or directories\n"
                  << "7. Sort directory content\n"
                  << "8. Quit\n"
                  << "Enter your choice: " << std::endl;
        std::getline(std::cin, command);

        int option = -1;
        try {
            option = std::stoi(command);
        } catch (const std::invalid_argument&) {
            std::cerr << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        switch (option) {
            case 1: {
                std::cout << "Enter navigation command (ls, cd <dir>, cd ..): ";
                std::getline(std::cin, command);

                if (command == "ls") {
                    listDirContents();
                } else if (command == "cd ..") {
                    goToParentDirectory();
                } else if (command.rfind("cd ", 0) == 0) {  // command starts with "cd "
                    std::string newPath = command.substr(3);
                    changeDir(newPath);
                } else {
                    std::cerr << "Unknown command: " << command << std::endl;
                }
                break;
            }
            case 2: {
                std::cout << "Enter creation command (touch <filename>, mkdir <dirname>): ";
                std::getline(std::cin, command);

                if (command.rfind("touch ", 0) == 0) {  // command starts with "touch "
                    std::string fileName = command.substr(6);
                    createFile(fileName);
                } else if (command.rfind("mkdir ", 0) == 0) {  // command starts with "mkdir "
                    std::string dirName = command.substr(6);
                    createDir(dirName);
                } else {
                    std::cerr << "Unknown command: " << command << std::endl;
                }
                break;
            }
            case 3: {
                listDirContents();
                std::cout << "Enter delete command (del <filename>, rmdir <dirname>): ";
                std::getline(std::cin, command);

                if (command.rfind("del ", 0) == 0) {  // command starts with "del "
                    std::string fileName = command.substr(4);
                    removeFile(fileName);
                } else if (command.rfind("rmdir ", 0) == 0) {  // command starts with "rmdir "
                    std::string dirName = command.substr(6);
                    removeDir(dirName);
                }
                break;
            }
            case 4: {
                std::cout << "Enter rename command (rename <oldname> <newname>): ";
                std::getline(std::cin, command);

                if (command.rfind("rename ", 0) == 0) {
                    std::istringstream iss(command.substr(7));
                    std::string oldName, newName;
                    iss >> oldName >> newName;
                    if (!oldName.empty() && !newName.empty()) {
                        renameFileOrDir(oldName, newName);
                    } else {
                        std::cerr << "Invalid rename command format. Use: rename <oldname> <newname>" << std::endl;
                    }
                }
                break;
            }
            case 5: {
                std::cout << "Enter read or write command (read <filename>, write <filename>): ";
                std::getline(std::cin, command);

                if (command.rfind("read ", 0) == 0) {  // Command starts with "read "
                    std::string fileName = command.substr(5);
                    readFile(fileName);
                } else if (command.rfind("write ", 0) == 0) {  // Command starts with "write "
                    std::string fileName = command.substr(6);
                    std::string content;
                    std::cout << "Enter content to write to file: ";
                    std::getline(std::cin, content);
                    writeFile(fileName, content);
                }
                break;
            }
            case 6: {
                while (true) {
                    std::cout << "Choose an option:\n"
                              << "1. Search for specific file by name\n"
                              << "2. Search for files by extension\n"
                              << "3. BACK\n" << std::endl;
                    std::getline(std::cin, command);

                    int searchOption = -1;
                    try {
                        searchOption = std::stoi(command);
                    } catch (const std::invalid_argument&) {
                        std::cerr << "Invalid input. Please enter a number." << std::endl;
                        continue;
                    }

                    switch (searchOption) {
                        case 1: {
                            std::string searchTerm;
                            std::cout << "Enter file name to search: " << std::endl;
                            std::getline(std::cin, searchTerm);
                            std::vector<std::string> files = searchFiles(searchTerm);
                            for (const auto& file : files) {
                                std::cout << file << std::endl;
                            }
                            break;
                        }
                        case 2: {
                            std::string extension;
                            std::cout << "Enter the file extension to search (e.g., .txt): ";
                            std::getline(std::cin, extension);
                            std::vector<std::string> filesByExtension = searchByExtension(extension);
                            std::cout << "Found files with extension " << extension << ":\n";
                            for (const auto& file : filesByExtension) {
                                std::cout << file << std::endl;
                            }
                            break;
                        }
                        case 3:
                            return;  // Go back to the previous menu
                        default:
                            std::cerr << "Unknown option: " << searchOption << std::endl;
                            break;
                    }
                }
                break;
            }
            case 7: {
                std::cout << "Choose sort criteria:\n"
                          << "1. Name\n"
                          << "2. Date Modified\n"
                          << "3. Size\n"
                          << "4. Type\n"
                          << "Enter your choice: " << std::endl;
                std::getline(std::cin, command);

                SortCriteria criteria;
                int sortOption = -1;
                try {
                    sortOption = std::stoi(command);
                } catch (const std::invalid_argument&) { 
                    std::cerr << "Invalid input. Please enter a number!" << std::endl;
                    continue;
                }

                switch (sortOption) {
                    case 1: 
                        criteria = SortCriteria::NAME;
                        break;
                    case 2:
                        criteria = SortCriteria::DATE_MODIFIED;
                        break;
                    case 3:
                        criteria = SortCriteria::SIZE;
                        break;
                    case 4:
                        criteria = SortCriteria::TYPE;
                        break;
                    default:
                        std::cerr << "Unknown sort criteria: " << sortOption << std::endl;
                        continue;
                }

                std::vector<fs::path> sortedEntries = sortFiles(criteria);
                std::cout << "Sorted directory content:" << std::endl;
                for (const auto& entry : sortedEntries) { 
                    std::cout << entry.filename().string() << "\n";
                }
                break;
            }
            case 8:
                return;  // Quit the program
            default:
                std::cerr << "Unknown choice: " << option << std::endl;
                break;
        }
    }
}


int main() { 

    displayCurrentDirectory();

    while(true) { 
        handleUserInput();
        displayCurrentDirectory();
    }

    return 0;
   
}




