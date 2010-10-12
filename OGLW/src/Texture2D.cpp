#include "../include/Texture2D.h"

Texture2D::Texture2D( char * filename, int width, int height, int wrap )
{
    LoadTexture( filename, width, height, wrap );
}
Texture2D::~Texture2D()
{
    glDeleteTextures( 1, &texture );
}
GLuint Texture2D::LoadTexture( char * filename, int width, int height, int wrap )
{
    //glBindTexture(GL_TEXTURE_2D, 13);
    /*glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);*/

    //glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    int x = 1;
    x = loadTGA (filename, 1); /* id is the texture id to bind too */

    return texture;
}
void Texture2D::Draw()
{
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
