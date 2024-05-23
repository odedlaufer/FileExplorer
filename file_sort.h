#ifndef FILE_SORT_H
#define FILE_SORT_H

#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

enum class SortCriteria {
    NAME,
    DATE_MODIFIED,
    SIZE,
    TYPE
};

std::vector<fs::path> sortFiles(SortCriteria criteria);




#endif /* FILE_SORT_H */
