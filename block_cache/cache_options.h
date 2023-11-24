//
// Created by lixinzhuang1 on 2023/11/21.
//

#ifndef BE_UNIT_TEST_CACHE_OPTIONS_H
#define BE_UNIT_TEST_CACHE_OPTIONS_H

#include <string>
#include <vector>

namespace starrocks {

    struct DirSpace {
        std::string path;
        size_t size;
    };

    struct CacheOptions {
        // basic
        size_t mem_space_size;
        std::vector<DirSpace> disk_spaces;
        std::string meta_path;

        // advanced
        size_t block_size;
        bool checksum;
        size_t max_concurrent_inserts;
        // The following options are only valid for cache lib engine currently
        size_t max_parcel_memory_mb;
        uint8_t lru_insertion_point;

    };

}


#endif //BE_UNIT_TEST_CACHE_OPTIONS_H
