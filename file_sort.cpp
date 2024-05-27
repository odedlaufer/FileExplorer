#include "file_sort.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>

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
    
    std::cout << "Common file extensions:\n";
    for (const auto& ext : extensions) {
        std::cout << ext << " | ";
    }
    std::cout << "\n";
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

        // std::copy_if(entries.begin(), entries.end(), std::back_inserter(filteredEntries), [](const fs::path& file) {
        //     auto ftime = fs::last_write_time(file);
        //     // Convert to std::time_t
        //     auto cftime = decltype(ftime)::clock::to_time_t(ftime);

        // });
        break;

    case FilterCriteria::SIZE:

        std::copy_if(entries.begin(), entries.end(), std::back_inserter(filteredEntries), [&](const fs::path& file) {
            return fs::is_regular_file(file) && fs::file_size(file) >= options.minSize && fs::file_size(file) <= options.maxSize;
        });
        break;
    
    case FilterCriteria::TYPE:
            
            printCommonExtensions();
            break;
    
    default:
        break;
    }


    return entries;
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
    
