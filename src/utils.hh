#pragma once

#include <expected>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>


inline bool file_exists(const std::filesystem::path& path) {
    return std::filesystem::exists(path);
}

// TODO: LOL
inline std::stringstream file_to_string(const std::filesystem::path& path) {
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer;
}

// Generic Errors
enum class GenericErr {
    Fail
};

template <typename T, typename E> bool is_expected_values(std::vector<std::expected<T, E>> items) {
    for (const auto& item : items) {
        if (!item) {
            return false;
        }
    }
    return true;
}

template <typename T, typename E>
inline std::vector<T> get_expected_values(std::vector<std::expected<T, E>> items) {
    std::vector<T> vs{};
    vs.reserve(items.size());

    for (const auto& item : items) {
        vs.push_back(item.value());
    }

    return vs;
}

template <typename T, typename E> inline T get_expected_value(std::expected<T, E> item) {
    return item.get();
}