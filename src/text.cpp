#include "text.hpp"
#include <glad/gl.h>
#include <iostream>

Text::Text(const char* vertexPath, const char* fragmentPath)
    :Shader{0,0,vertexPath, fragmentPath}
{
    
    if(FT_Init_FreeType(&ft))
    {
        std::cerr << "couldn't init freetype!" << std::endl;
        std::exit(1);
    }

    if(FT_New_Face(ft, "assets/FreeSans.ttf", 0, &face))
    {
        std::cerr << "couldn't open font!" << std::endl;
        std::exit(1);
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Text::renderText(const char* text, float x, float y,
        float sx, float sy)
{
    FT_Set_Pixel_Sizes(face, 0, 48);
    if (textVAO == 0)
    {
        glActiveTexture(GL_TEXTURE31);
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        use();
        setInt("tex", 31);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenVertexArrays(1, &textVAO);

        glBindVertexArray(textVAO);
        glGenBuffers(1, &textVBO);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
    }
    glActiveTexture(GL_TEXTURE31);
    glBindTexture(GL_TEXTURE_2D, tex);

    const char* p;
    for(p = text; *p; p++)
    {
        if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
        {
            continue;
        }

        FT_GlyphSlot g = face->glyph;

        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                g->bitmap.width,
                g->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                g->bitmap.buffer);

        float x2 = x + g->bitmap_left * sx;
        float y2 = -y - g->bitmap_top * sy;
        float w = g->bitmap.width * sx;
        float h = g->bitmap.rows * sy;

        GLfloat box[4][4] = {
            {x2,     -y2    , 0, 0},
            {x2 + w, -y2    , 1, 0},
            {x2,     -y2 - h, 0, 1},
            {x2 + w, -y2 - h, 1, 1},
        };

        glBindVertexArray(textVAO);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        x += (g->advance.x/64.0f) * sx;
        y += (g->advance.y/64.0f) * sy;

    }
    glActiveTexture(0);

}


 
