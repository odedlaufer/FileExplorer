#ifndef FILE_SORT_H
#define FILE_SORT_H

#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

enum class SortCriteria {
    NAME,
    DATE_MODIFIED,
    SIZE,
    TYPE
};

enum class FilterCriteria { 
    TYPE,
    DATE_CREATED,
    SIZE
};

struct FilterOptions { 

    FilterCriteria criteria;
    std::string extension;
    std::size_t dateRange;
    std::size_t minSize;
    std::size_t maxSize;
};

std::vector<fs::path> sortFiles(SortCriteria criteria);

std::vector<fs::path> filterFiles(const FilterOptions& options);

std::vector<std::string> getTopFileExtensions(int topCount);

void printCommonExtensions();






#endif /* FILE_SORT_H */
