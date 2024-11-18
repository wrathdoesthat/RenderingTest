#pragma once

#include <cstdint>
#include <filesystem>

#include "glad/glad.h"
#include "stb_image.h"

struct Texture {
    // They store the ids as uints so im doing it too to prevent conversion
    uint32_t id{0};
    int32_t width{0};
    int32_t height{0};
    int32_t channels{0};
};

bool create_texture(const std::filesystem::path& path, Texture& created_texture);