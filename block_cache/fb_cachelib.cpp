//
// Created by lixinzhuang1 on 2023/11/24.
//

#include "block_cache/fb_cachelib.h"
#include "util/filesystem_util.h"
#include "common/logging.h"

namespace starrocks {

    Status FbCacheLib::init(const CacheOptions& options) {
        Cache::Config config;
        config.setCacheSize(options.mem_space_size).setCacheName("default cache").setAccessConfig({25, 10}).validate();

        std::vector<std::string> nvm_files;

        if(!options.disk_spaces.empty()) {
            Cache::NvmCacheConfig nvmConfig;
            nvmConfig.navyConfig.setBlockSize(4096);

            for (auto& dir : options.disk_spaces) {
                nvm_files.emplace_back(dir.path + "/cachelib_data");
                // If exist, truncate it to empty file
                FileSystemUtil::resize_file(nvm_files.back(), 0);
            }

            if(nvm_files.size() == 1) {
                nvmConfig.navyConfig.setSimpleFile(nvm_files[0], options.disk_spaces[0].size, false);
            } else {
                nvmConfig.navyConfig.setRaidFiles(nvm_files, options.disk_spaces[0].size, false);
            }

            nvmConfig.navyConfig.blockCache().setRegionSize(16 * 1024 * 1024);
            nvmConfig.navyConfig.blockCache().setDataChecksum(options.checksum);
            nvmConfig.navyConfig.setMaxParcelMemoryMB(options.max_parcel_memory_mb);
            nvmConfig.navyConfig.setMaxConcurrentInserts(options.max_concurrent_inserts);
            config.enableNvmCache(nvmConfig);
        }

        Cache::MMConfig mm_config;
        mm_config.lruInsertionPointSpec = options.lru_insertion_point;
        _cache = std::make_unique<Cache>(config);
        _default_pool = _cache ->addPool("default pool", _cache -> getCacheMemoryStats().cacheSize, {}, mm_config);
        _meta_path = options.meta_path;
        return Status::OK();
    }
}



