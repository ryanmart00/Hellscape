#ifndef TEXT_HPP
#define TEXT_HPP

#include "shader.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/config/ftheader.h>


class Text : public Shader
{
public:
    Text() = delete;
    Text(const char* vertexPath, const char* fragmentPath);
    void renderText(const char* text, float x, float y,
        float sx, float sy);
private:
    FT_Library ft;
    FT_Face face;
    uint tex;
    uint textVAO = 0;
    uint textVBO = 0;
};
#endif
