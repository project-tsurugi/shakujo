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
#include "shakujo/model/Node.h"

#include <gtest/gtest.h>

#include "shakujo/model/IRFactory.h"

namespace shakujo::model {

class NodeTest : public ::testing::Test {};

using Region = shakujo::common::core::DocumentRegion;

IRFactory f;

TEST_F(NodeTest, inject_region) {
    auto name = f.Name("name");
    auto ptr = name.get();

    auto injected = std::move(name) << Region("testing");
    EXPECT_EQ(ptr, injected.get());
    EXPECT_EQ("testing", injected->region().path());
}

}  // namespace shakujo::model
