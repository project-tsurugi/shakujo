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
#ifndef SHAKUJO_COMMON_UTIL_UTILITY_H_
#define SHAKUJO_COMMON_UTIL_UTILITY_H_

#include <memory>
#include <utility>
#include <stdexcept>
#include <string>
#include <sstream>
#include <type_traits>
#include <vector>

namespace shakujo::common::util {

/**
 * @brief returns whether or not the given pointer like has value.
 * @tparam T the pointer type
 * @param ptr the pointer
 * @return true if the pointer is not null
 * @return false otherwise
 */
template<class T>
inline constexpr bool is_defined(T const* ptr) {
    return ptr != nullptr;
}

/**
 * @brief returns whether or not the given pointer like object has value.
 * @tparam T the pointer type
 * @param ptr the pointer like object
 * @return true if the object does not refer null
 * @return false otherwise
 */
template<class T>
inline std::enable_if_t<!std::is_pointer_v<T>, decltype(is_defined(std::declval<T>().get()))>
is_defined(T const& ptr) {
    return is_defined(ptr.get());
}

/**
 * @brief returns whether or not the given object is valid.
 * @tparam T the object type
 * @param object the object
 * @return true if the object is valid
 * @return false otherwise
 */
template<class T>
inline auto is_valid(T const& object) -> decltype(object.is_valid()) {
    return object.is_valid();
}

/**
 * @brief returns whether or not the given pointer like object is valid.
 * @tparam T the pointer type
 * @param object the pointer like object
 * @return true if the object is not null and is valid
 * @return false otherwise
 */
template<class T>
inline auto is_valid(T const& object) -> decltype(is_defined(object), object->is_valid(), bool {}) {
    return is_defined(object) && object->is_valid();
}

/**
 * @brief returns whether or not the both pointer like objects are equivalent.
 * @tparam S the first pointer type
 * @tparam T the second pointer type
 * @param s the first pointer like object
 * @param t the second pointer like object
 * @return true if the both are same reference or both are equivalent
 * @return false otherwise
 */
template<class S, class T>
inline std::enable_if_t<
    std::is_convertible_v<decltype(!std::declval<S>()), bool>
        && std::is_convertible_v<decltype(!std::declval<T>()), bool>
        && std::is_convertible_v<decltype(*std::declval<S>() == *std::declval<T>()), bool>,
    bool
>
equals(S&& s, T&& t) {
    if (!s) {
        return !t;
    }
    if (!t) {
        return false;
    }
    return *s == *t;
}

/**
 * @brief apply dynamic_cast to raw pointer.
 * @tparam T the destination object type
 * @tparam U the source object type
 * @param ptr the source pointer
 * @return the applied pointer
 * @throws std::bad_alloc if failed to cast
 */
template<class T, class U>
inline std::add_pointer_t<std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>>
dynamic_pointer_cast(U* ptr) {
    using ret_t = std::add_pointer_t<std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>>;
    if (!is_defined(ptr)) {
        return {};
    }
    if (auto* raw = dynamic_cast<ret_t>(ptr)) {
        return raw;
    }
    throw std::bad_cast();
}

/**
 * @brief apply dynamic_cast to unique_ptr.
 * @tparam T the destination object type
 * @tparam U the source object type
 * @param ptr the source pointer
 * @return the applied pointer
 * @throws std::bad_alloc if failed to cast
 */
template<class T, class U>
inline std::unique_ptr<std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>>
dynamic_pointer_cast(std::unique_ptr<U> ptr) {
    using object_t = std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>;
    if (!is_defined(ptr)) {
        return {};
    }
    if (auto* raw = dynamic_cast<object_t*>(ptr.get())) {
        std::unique_ptr<object_t> result { raw };
        ptr.release();
        return result;
    }
    throw std::bad_cast();
}

/**
 * @brief apply dynamic_cast to raw pointer only if the actual type is the specified.
 * @tparam T the destination object type
 * @tparam U the source object type
 * @param ptr the source pointer
 * @return the applied pointer
 * @return nullptr if failed to cast
 */
template<class T, class U>
inline std::add_pointer_t<std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>>
dynamic_pointer_cast_if(U* ptr) {
    using ret_t = std::add_pointer_t<std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>>;
    return dynamic_cast<ret_t>(ptr);
}

/**
 * @brief apply dynamic_cast to unique_ptr only if the actual type is the specified.
 * @tparam T the destination object type
 * @tparam U the source object type
 * @param ptr the source pointer
 * @return the applied pointer
 * @throws empty if failed to cast
 */
template<class T, class U>
inline std::unique_ptr<std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>>
dynamic_pointer_cast_if(std::unique_ptr<U>& ptr) {
    using object_t = std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>;
    if (auto* raw = dynamic_cast<object_t*>(ptr.get())) {
        std::unique_ptr<object_t> result { raw };
        ptr.release();
        return result;
    }
    return {};
}

/**
 * @brief constructs a new clone of the given object.
 * @tparam T the element pointer type
 * @param ptr the source object pointer
 * @return the created clone
 */
template<class T>
inline auto
make_clone(T&& ptr) -> std::unique_ptr<std::remove_pointer_t<decltype(is_defined(ptr), ptr->clone())>> {
    if (!is_defined(ptr)) {
        return {};
    }
    using element_type = std::remove_pointer_t<decltype(ptr->clone())>;
    return std::unique_ptr<element_type> { ptr->clone() };
}

/**
 * @brief constructs a new clone of the given object.
 * @tparam T the element type
 * @param ptr the source object pointer
 * @return the created clone
 */
template<class T>
inline auto
make_clone(T&& ptr) -> std::unique_ptr<std::remove_pointer_t<decltype(ptr.clone())>> {
    using element_type = std::remove_pointer_t<decltype(ptr.clone())>;
    return std::unique_ptr<element_type> { std::forward<T>(ptr).clone() };
}

/**
 * @brief appends nothing.
 */
inline void append(std::ostream&) {}

/**
 * @brief appends values into the stream.
 * @tparam First the first value type
 * @tparam Args the rest value types
 * @param out the target stream
 * @param first the first value
 * @param rest the rest values
 */
template<class First, class... Args>
inline std::enable_if_t<!std::is_array_v<First> && !(std::is_pointer_v<First> && std::is_class_v<std::remove_pointer_t<First>>)>
append(std::ostream& out, First const& first, Args const&... rest);

/**
 * @brief appends values into the stream.
 * @tparam First the first value type
 * @tparam Args the rest value types
 * @param out the target stream
 * @param first the first value
 * @param rest the rest values
 */
template<class First, class... Args>
inline std::enable_if_t<std::is_class_v<First>>
append(std::ostream& out, First const* first, Args const&... rest);

/**
 * @brief appends values into the stream.
 * @tparam First the first value type
 * @tparam N the number of elements in the first value array
 * @tparam Args the rest value types
 * @param out the target stream
 * @param first the first value
 * @param rest the rest values
 */
template<class First, std::size_t N, class... Args>
inline void append(std::ostream& out, First const (&first)[N], Args const&... rest);

/**
 * @brief appends values into the stream.
 * @tparam FirstElement the element type of first value
 * @tparam Args the rest value types
 * @param out the target stream
 * @param first the first value
 * @param rest the rest values
 */
template<class FirstElement, class... Args>
inline void append(std::ostream& out, std::vector<FirstElement> const& first, Args const&... rest);

// FIXME: doxygen cannot not detect the declaration of this definition...
/**
 * @brief appends values into the stream.
 * @tparam First the first value type
 * @tparam Args the rest value types
 * @param out the target stream
 * @param first the first value
 * @param rest the rest values
 */
template<class First, class... Args>
inline std::enable_if_t<!std::is_array_v<First> && !(std::is_pointer_v<First> && std::is_class_v<std::remove_pointer_t<First>>)>
append(std::ostream& out, First const& first, Args const&... rest) {
    out << first;
    append(out, rest...);
}

template<class First, class... Args>
inline std::enable_if_t<std::is_class_v<First>>
append(std::ostream &out, First const* first, Args const &... rest) {
    if (!first) {
        out << "(undefined)";
    } else {
        out << *first;
    }
    append(out, rest...);
}

template<class First, std::size_t N, class... Args>
inline void append(std::ostream& out, First const (&first)[N], Args const&... rest) {
    out << &first[0];
    append(out, rest...);
}

template<class FirstElement, class... Args>
inline void append(std::ostream& out, std::vector<FirstElement> const& first, Args const&... rest) {
    bool saw_element = false;
    out << "[";
    for (auto& element : first) {
        if (saw_element) {
            out << ", ";
        }
        saw_element = true;
        append(out, element);
    }
    out << "]";
    append(out, rest...);
}

/**
 * @brief converts a sequence of values into a string.
 * @tparam Args the value types
 * @param args the values
 * @return the corresponded string
 */
template<class... Args>
inline std::string to_string(Args const&... args) {
    std::ostringstream ss;
    append(ss, args...);
    return ss.str();
}

/**
 * @brief returns the number of bytes in the given null terminated string.
 * If the string does not contain null characters until the given size, this returns its size.
 * @param string the target null terminated string
 * @param size the maximum string size
 * @return the number of bytes before the null characters, if the string contains null characters
 * @return == size if the string does not contain any null characters
 * @return == 0 if string is null
 */
static inline std::size_t strnlen(char const * string, std::size_t size) {
    if (!string) {
        return 0U;
    }
    std::size_t length = 0;
    while (*string++ && size != 0) {  // NOLINT
        ++length;
        --size;
    }
    return length;
}

}  // namespace shakujo::common::util

#endif  // SHAKUJO_COMMON_UTIL_UTILITY_H_
