//
// Created by lixinzhuang1 on 2023/11/24.
//

#ifndef BE_UNIT_TEST_FB_CACHELIB_H
#define BE_UNIT_TEST_FB_CACHELIB_H

#include <cachelib/allocator/CacheAllocator.h>

#include "block_cache/kv_cache.h"
#include "common/status.h"

namespace starrocks {
    class FbCacheLib : public KvCache {

    public:
        using Cache = facebook::cachelib::LruAllocator;
        using PoolId = facebook::cachelib::PoolId;
        using ReadHandle = facebook::cachelib::LruAllocator::ReadHandle;

        FbCacheLib() = default;
        ~FbCacheLib() override = default;

        Status init(const CacheOptions& options) override;
        Status write_cache(const std::string& key, const char* value, size_t size, size_t ttl_seconds) override;

        size_t read_cache(const std::string& key, char* value, size_t off, size_t size) override;

        Status remove_cache(const std::string& key) override;

        std::unordered_map<std::string, double> cache_stats() override;

        Status shutdown() override;

    private:
        void _dump_cache_stats();

        std::unique_ptr<Cache> _cache = nullptr;
        PoolId _default_pool;
        std::string _meta_path;
    };
}


#endif //BE_UNIT_TEST_FB_CACHELIB_H
