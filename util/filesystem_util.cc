// This file is made available under Elastic License 2.0.
// This file is based on code available under the Apache license here:
//   https://github.com/apache/incubator-doris/blob/master/be/src/util/filesystem_util.cc

// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "util/filesystem_util.h"

#include <fcntl.h>
#include <sys/resource.h>
#include <sys/stat.h>

#include <filesystem>
#include <system_error>

// #include "util/error_util.h"

using std::exception;
using std::string;
using std::vector;

// std::filesystem functions must be given an errcode parameter to avoid the variants
// of those functions that throw exceptions.
namespace starrocks {



Status FileSystemUtil::resize_file(const string& file_path, int64_t trunc_len) {
    int success = truncate(file_path.c_str(), trunc_len);
    if (success != 0) {
        std::stringstream error_msg;
        error_msg << "Truncate file " << file_path << " to length " << trunc_len << " failed with " << errno << " ("
                  << ")";
        return Status::InternalError(error_msg.str());
    }

    return Status::OK();
}



} // end namespace starrocks
