/*
 * Copyright 2018-2023 Project Tsurugi.
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
#ifndef SHAKUJO_MODEL_UTIL_NODE_LIST_H_
#define SHAKUJO_MODEL_UTIL_NODE_LIST_H_

#include <memory>

#include "NodeListBase.h"

namespace shakujo::model::util {

/**
 * @brief A list of IR node.
 * @tparam E the node type
 */
template<typename E>
using NodeList = NodeListBase<std::unique_ptr<E>>;

}  // namespace shakujo::model::util

#endif  // SHAKUJO_MODEL_UTIL_NODE_LIST_H_
