cmake_minimum_required(VERSION 3.17)
project(be_unit_test)

set(CMAKE_CXX_STANDARD 17)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -gdwarf-2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -gdwarf-2")

include(FetchContent)
#find_package (glog 0.6.0 REQUIRED)


#FetchContent_Declare(
#        googletest
#        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest
#)
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)
#enable_testing()

find_package(Threads REQUIRED)
#find_package(fmt REQUIRED)
FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG master
        )
FetchContent_MakeAvailable(fmt)

set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

set(CACHELIB_DEPENDENCIES
        cachelib_common
        cachelib_allocator
        cachelib_datatype
        cachelib_navy
        cachelib_shm
        folly
        fmtd
        thriftcpp2
        thrift-core
        thriftmetadata
        thriftfrozen2
        thriftprotocol
        thrifttype
        transport
        rpcmetadata
        thriftanyrep
        thrifttyperep
        thriftannotation
        concurrency
        async
        wangle
        )

foreach(dep ${CACHELIB_DEPENDENCIES})
    add_library(${dep} STATIC IMPORTED)
    set(location "${THIRDPARTY_DIR}/cachelib/lib/lib${dep}.a")
    if(NOT EXISTS ${location})
        set(location "${THIRDPARTY_DIR}/cachelib/lib64/lib${dep}.a")
    endif()
    set_target_properties(${dep} PROPERTIES IMPORTED_LOCATION "${location}")
    message(STATUS "add cachelib dependency: ${location}")
endforeach()

# 源文件打包成库
add_executable(be_unit_test
        main.cpp
        block_cache/block_cache.cpp
        test/block_cache_test.cpp
        #common/configbase.cpp
        #common/statusor.cpp
        block_cache/fb_cachelib.cpp
        util/filesystem_util.cc)

# 包含头文件
target_include_directories(
        be_unit_test
        PRIVATE
        ./
        ./third_party/include
        ./third_party/cachelib/include
)

add_library(gtest STATIC IMPORTED)
set_target_properties(gtest PROPERTIES IMPORTED_LOCATION ${THIRDPARTY_DIR}/lib/libgtest.a)

add_library(gtest_main STATIC IMPORTED)
set_target_properties(gtest_main PROPERTIES IMPORTED_LOCATION ${THIRDPARTY_DIR}/lib/libgtest_main.a)

add_library(glog STATIC IMPORTED)
set_target_properties(glog PROPERTIES IMPORTED_LOCATION ${THIRDPARTY_DIR}/lib/libglog.a)

add_library(gflags STATIC IMPORTED)
set_target_properties(gflags PROPERTIES IMPORTED_LOCATION ${THIRDPARTY_DIR}/lib/libgflags.a)

# 连接外部文件
target_link_libraries(
        be_unit_test
        PRIVATE
        Threads::Threads
        gtest
        gtest_main
        glog
        gflags
        #glog::glog
        fmt::fmt
)
