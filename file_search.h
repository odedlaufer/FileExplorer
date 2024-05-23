#ifndef FILE_SEARCH_H
#define FILE_SEARCH_H
#include <vector>
#include <string>


std::vector<std::string> searchFiles(const std::string& searchTerm);
std::vector<std::string> searchByExtension(const std::string& extension);

#endif /* FILE_SEARCH_H */
