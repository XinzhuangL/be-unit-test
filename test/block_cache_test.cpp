
#include "block_cache/block_cache.h"

#include <gtest/gtest.h>

#include <cstring>

namespace starrocks {
class BlockCacheTest : public ::testing::Test {

};

TEST_F(BlockCacheTest, init_test) {
    BlockCache* bCache = BlockCache::instance();
    CacheOptions options;
    bCache->init(options);
}

}
