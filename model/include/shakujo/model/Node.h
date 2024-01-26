/*
 * Copyright 2018-2024 Project Tsurugi.
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
#ifndef SHAKUJO_MODEL_NODE_H_
#define SHAKUJO_MODEL_NODE_H_

#include "shakujo/common/core/DocumentRegion.h"
#include "shakujo/common/util/Clonable.h"

namespace shakujo::model {
/**
 * @brief An abstract super interface of IR model node.
 */
class Node : public common::util::Clonable {
private:
    common::core::DocumentRegion region_;

protected:
    /**
     * @brief Constructs a new object.
     */
    Node() noexcept = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Node(const Node& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Node(Node&& other) noexcept = default;

    /**
     * @brief Copy assigns to this object.
     * @param other the source object
     * @return this
     */
    Node& operator=(const Node& other) = default;

    /**
     * @brief Move assigns to this object.
     * @param other the source object
     * @return this
     */
    Node& operator=(Node&& other) noexcept = default;

public:
    ~Node() noexcept override = default;

    Node* clone() const & override = 0;

    Node* clone() && override = 0;

    /**
     * @brief returns the document region.
     * @return the document region
     */
    const common::core::DocumentRegion& region() const {
        return region_;
    }

    /**
     * @brief sets the document region.
     * @param region the document region
     * @return this
     */
    Node& region(common::core::DocumentRegion region) {
        region_ = std::move(region);
        return *this;
    }
};
}  // namespace shakujo::model

#endif  // SHAKUJO_MODEL_NODE_H_
