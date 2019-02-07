/*
 * Copyright 2018-2018 shakujo project.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <gtest/gtest.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

namespace {
class Env : public ::testing::Environment {
public:
    Env() noexcept = default;
    ~Env() override = default;
    Env(Env const&) = default;
    Env(Env&&) noexcept = default;
    Env& operator=(Env const&) = default;
    Env& operator=(Env&&) noexcept = default;

    void SetUp() override {
        // print to stderr if GLOG_log_dir is not set explicitly
        if (FLAGS_log_dir.empty()) {
            FLAGS_logtostderr = true;
        }
        ::google::InitGoogleLogging(::gflags::GetArgv0());
        ::google::InstallFailureSignalHandler();
    }

    void TearDown() override {
        ::google::ShutdownGoogleLogging();
    }
};
}  // namespace

int main(int argc, char** argv) {
    // first consume command line options for gtest
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new Env());  // NOLINT

    // parse command line arguments using gflags
    ::gflags::SetArgv(argc, const_cast<char const**>(argv));
    ::gflags::ParseCommandLineFlags(&argc, &argv, false);

    return RUN_ALL_TESTS();
}
