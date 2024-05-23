#include "file_sort.h"
#include <algorithm>
#include <iostream>

std::vector<fs::path> sortFiles(SortCriteria criteria) { 

    std::vector<fs::path> entries;
    fs::path directory = fs::current_path();

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
    
