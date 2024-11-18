#include "texture.hh"

bool create_texture(const std::filesystem::path& path, Texture& created_texture) {
    int image_width, image_height, chans;
    unsigned char* data = stbi_load(path.string().c_str(), &image_width,
                                    &image_height, &chans, 0);

    uint32_t format_type{0};
    switch (chans) {
    case 3:
        format_type = GL_RGB;
        break;
    case 4:
        format_type = GL_RGBA;
        break;
    default: {
        // Lol
    }
    }

    uint32_t tex_id{0};
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format_type, image_width, image_height, 0,
                 format_type, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    created_texture = Texture{
        .id = tex_id,
        .width = image_width,
        .height = image_height,
        .channels = chans,
    };

    stbi_image_free(data);
    return true;
}