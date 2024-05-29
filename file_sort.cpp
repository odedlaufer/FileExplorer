#include "file_sort.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <chrono>
#include <iomanip>
#include <sstream>

fs::path directory = fs::current_path();
int TOP_EXT_COUNT = 10;

std::vector<std::string> getTopFileExtensions() { 

    //RETRIEVE EXTENSIONS
    std::unordered_map<std::string, int> extensionCount;

    for(const auto& entry : fs::directory_iterator(directory)) { 

        if(entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            if(!extension.empty()) { 
                extensionCount[extension]++;
            }
        }
    }

    // SORT EXTENSIONS DESC
    std::vector<std::pair<std::string, int>> sortedExtensions(extensionCount.begin(), extensionCount.end());
    std::sort(sortedExtensions.begin(), sortedExtensions.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    // SELECT TOP COUNT
    std::vector<std::string> topExtensions;
    for (int i = 0; i < std::min(TOP_EXT_COUNT, static_cast<int>(sortedExtensions.size())); ++i) {
        topExtensions.push_back(sortedExtensions[i].first);
    }

    return topExtensions;

}

void printCommonExtensions() {
    std::vector<std::string> extensions = getTopFileExtensions();
    
    for (const auto& ext : extensions) {
        std::cout << ext << " | ";
    }
    std::cout << "\n";
}

// Helper function to parse date from string
std::chrono::system_clock::time_point parseDate(const std::string& dateStr) {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

std::vector<fs::path> filterFiles(const FilterOptions& options) { 

    std::vector<fs::path> entries, filteredEntries;
    std::vector<std::string> extensions = getTopFileExtensions();

    for(const auto& entry : fs::directory_iterator(directory)) { 
        entries.push_back(entry.path());
    }

    switch (options.criteria)
    {
    case FilterCriteria::DATE_CREATED: 
    
        std::copy_if(entries.begin(), entries.end(), std::back_inserter(filteredEntries), [&](const fs::path& file) {
        auto ftime = fs::last_write_time(file);
        auto cftime = decltype(ftime)::clock::to_time_t(ftime);
        auto fileTime = std::chrono::system_clock::from_time_t(cftime);
        return fileTime >= options.startDate && fileTime <= options.endDate;
        });
        break;

    case FilterCriteria::SIZE:

        std::copy_if(entries.begin(), entries.end(), std::back_inserter(filteredEntries), [&](const fs::path& file) {
            return fs::is_regular_file(file) && fs::file_size(file) >= options.minSize && fs::file_size(file) <= options.maxSize;
        });
        break;
    
    case FilterCriteria::TYPE:
            std::copy_if(entries.begin(), entries.end(), std::back_inserter(filteredEntries), [&](const fs::path& file) {
                return fs::is_regular_file(file) && file.extension() == options.extension;
            });
            break;
    
    default:
        break;
    }


    return filteredEntries;
}

std::vector<fs::path> sortFiles(SortCriteria criteria) { 

    std::vector<fs::path> entries;

    for(const auto& entry : fs::directory_iterator(directory)) { 
        entries.push_back(entry.path());
    }

    switch (criteria) { 
        case SortCriteria::NAME: {
            std::sort(entries.begin(), entries.end(), [](const fs::path& a, const fs::path& b) { 
                return a.filename().string() < b.filename().string();
            });
            break;
        }
        case SortCriteria::DATE_MODIFIED: { 
            std::sort(entries.begin(), entries.end(), [](const fs::path& a, const fs::path& b) { 
                return fs::last_write_time(a) < fs::last_write_time(b);
            });
            break;
        }
        case SortCriteria::SIZE: { 
            std::sort(entries.begin(), entries.end(), [](const fs::path& a, const fs::path& b) {
                return fs::file_size(a) < fs::file_size(b);
            });
            break;
        }
        case SortCriteria::TYPE: { 
            std::sort(entries.begin(), entries.end(), [](const fs::path& a, const fs::path& b) { 
                return a.extension().string() < b.extension().string();
            });
            break;
        }
    }
    return entries;
}
    
