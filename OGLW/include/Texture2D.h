#ifndef TEXTURE2D_H_INCLUDED
#define TEXTURE2D_H_INCLUDED
#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>
#include "tga.h"

class Texture2D
{
public :
    Texture2D(char * filename, int width, int height, int wrap );
    ~Texture2D();
    void Draw();
private:
    GLuint LoadTexture( char * filename, int width, int height, int wrap );

    GLuint texture;
};
#endif // TEXTURE2D_H_INCLUDED
