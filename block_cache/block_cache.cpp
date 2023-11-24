
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <filesystem>
#include "block_cache/block_cache.h"
#include "common/logging.h"
// #include "gutil/strings/substitute.h"

namespace starrocks {
    namespace fs = std::filesystem;

    BlockCache* BlockCache::instance() {
        static BlockCache cache;
        return &cache;
    }

    Status BlockCache::init(const starrocks::CacheOptions &options) {
        for (auto& dir : options.disk_spaces) {
            if (dir.size == 0) {
                continue;
            }
            fs::path dir_path(dir.path);
            if(fs::exists(dir_path)) {
                if(fs::is_directory(dir_path)) {
                    LOG(ERROR) << "the block cache disk path already exists but not a directory, path: " << dir.path;
                    return Status::InvalidArgument("invalid block cache disk path");
                }
            } else {
                std::error_code ec;
                if(!fs::create_directory(dir_path, ec)) {
                    LOG(ERROR) << "create block cache disk path failed, path: " << dir.path << ", reason: " << ec.message();
                    return Status::InvalidArgument("invalid block cache disk path");
                }
            }
        }
        _block_size = options.block_size;
        // todo 实例化FbCacheLib
        return _kv_cache ->init(options);
    }

    Status BlockCache::write_cache(const starrocks::BlockCache::CacheKey &cache_key, off_t offset, size_t size,
                                   const char *buffer, size_t ttl_seconds) {
        if(offset % _block_size != 0) {
            LOG(WARNING) << "write block key: " << cache_key << " with invalid args, offset: " << offset;
            return Status::InvalidArgument(&"offset must be aligned by block size " [ _block_size]);
        }
        if(!buffer) {
            return Status::InvalidArgument("invalid data buffer");
        }

        size_t index = offset / _block_size;
        std::string block_key = fmt::format("{}/{}", cache_key, index);
        return _kv_cache ->write_cache(block_key, buffer, size, ttl_seconds);
    }

    size_t BlockCache::read_cache(const starrocks::BlockCache::CacheKey &cache_key, off_t offset, size_t size,
                                            char *buffer) {
        // when buffer == nullptr, it can check if cached
        if(size == 0) {
            return 0;
        }
        size_t index = offset / _block_size;
        std::string block_key = fmt::format("{}/{}", cache_key, index);
        return _kv_cache ->read_cache(block_key, buffer, offset - index * _block_size, size);
    }

    Status BlockCache::remove_cache(const starrocks::BlockCache::CacheKey &cache_key, off_t offset, size_t size) {
        if (offset % _block_size != 0) {
            LOG(WARNING) << "remove block key: " << cache_key << " with invalid args, offset: " << offset
                         << ", size: " << size;
            return Status::InvalidArgument(
                    &"offset and size must be aligned by block size"  [_block_size]);
        }
        if (size == 0) {
            return Status::OK();
        }

        size_t index = offset / _block_size;
        std::string block_key = fmt::format("{}/{}", cache_key, index);
        return _kv_cache->remove_cache(block_key);
    }


    Status BlockCache::shutdown() {
        return _kv_cache->shutdown();
    }


}