/*
    Copyright (C) 1999
    For non-commercial use only.

    File	: tga.c
    Date	: 05/05/1999
    Author	: Nate Miller
    Contact	: vandals1@home.com

    Change Log
    **********
    7/26/99 - added code to fclose () a file when there is an error
    6/11/99 - added support for 8bit images, changed commenting
*/
#include "../include/tga.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>

GLenum texFormat;

/*
=============
checkSize

Make sure its a power of 2.
=============
*/
int checkSize (int x)
{
    if (x == 2	 || x == 4 ||
        x == 8	 || x == 16 ||
        x == 32  || x == 64 ||
        x == 128 || x == 256 || x == 512)
        return 1;
    else return 0;
}
/*
=============
getRGBA

Reads in RGBA data for a 32bit image.
=============
*/
unsigned char *getRGBA (FILE *s, int size)
{
    unsigned char *rgba;
    unsigned char temp;
    int bread;
    int i;

    rgba = (unsigned char*)malloc (size * 4);

    if (rgba == NULL)
        return 0;

    bread = fread (rgba, sizeof (unsigned char), size * 4, s);

    /* TGA is stored in BGRA, make it RGBA */
    if (bread != size * 4)
    {
        free (rgba);
        return 0;
    }

    for (i = 0; i < size * 4; i += 4 )
    {
        temp = rgba[i];
        rgba[i] = rgba[i + 2];
        rgba[i + 2] = temp;
    }

    texFormat = GL_RGBA;
    return rgba;
}


unsigned char *getData (FILE *s, int sz)
{
        return getRGBA (s, sz);
}
/*
=============
returnError
=============
Called when there is an error loading the .tga file.
*/
int returnError (FILE* s, int error)
{
    fclose (s);
    return error;
}
/*
=============
loadTGA

Loads up a targa file.  Supported types are 8,24 and 32 uncompressed images.
id is the texture ID to bind too.
=============
*/
int loadTGA (char *name, int id)
{
    unsigned char type[4];
    unsigned char info[7];
    unsigned char *imageData = NULL;
    int imageWidth, imageHeight;
    int size;
    FILE *s;

    if (!(s = fopen (name, "r+bt")))
        return TGA_FILE_NOT_FOUND;

    fread (&type, sizeof (char), 3, s); // read in colormap info and image type, byte 0 ignored
    fseek (s, 12, SEEK_SET);			// seek past the header and useless info
    fread (&info, sizeof (char), 6, s);

    if (type[1] != 0 || (type[2] != 2 && type[2] != 3))
        returnError (s, TGA_BAD_IMAGE_TYPE);

    imageWidth = info[0] + info[1] * 256;
    imageHeight = info[2] + info[3] * 256;

    size = imageWidth * imageHeight;

    /* make sure dimension is a power of 2 */
    if (!checkSize (imageWidth) || !checkSize (imageHeight))
        returnError (s, TGA_BAD_DIMENSION);

    imageData = getData (s, size);

    /* no image data */
    if (imageData == NULL)
        returnError (s, TGA_BAD_DATA);

    fclose (s);

    glBindTexture (GL_TEXTURE_2D, id);
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /* glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); */
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /* glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); */
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D (GL_TEXTURE_2D, 0, texFormat, imageWidth, imageHeight, 0, texFormat, GL_UNSIGNED_BYTE, imageData);

    /* release data, its been uploaded */
    free (imageData);

    return 1;
}

