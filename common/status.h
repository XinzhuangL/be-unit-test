// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#pragma once

#include <string>
#include <vector>

// #include "common/compiler_util.h"
#include "common/logging.h"
// #include "gen_cpp/StatusCode_types.h" // for TStatus
//#include "util/slice.h"               // for Slice
// #include "util/time.h"
#include <string>
namespace starrocks {

    class StatusPB;

    class TStatus;

    template<typename T>
    class StatusOr;

    class Status {
    public:
        Status(uint32_t code, const std::string msg) : _code(code), _msg(msg) {

        };

        static Status InvalidArgument(const std::string msg) { return Status(-1, msg); }

        static Status InternalError(const std::string msg) { return Status(-2, msg); }

        static Status OK() { return Status(0, ""); }

    private:
        uint32_t _code;
        const std::string _msg;

    };
}