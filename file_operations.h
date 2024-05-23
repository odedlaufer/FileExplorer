#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <string>

bool confirmAction(const std::string& actionType);
void createFile(const std::string& fileName);
bool createDir(const std::string& dirName);
void removeFile(const std::string& fileName);
bool removeDir(const std::string& dirName);
void renameFileOrDir(const std::string& oldName, const std::string& newName);
void readFile(const std::string& fileName);
void writeFile(const std::string& fileName, const std::string& content);


#endif // FILE_OPERATIONS_H
