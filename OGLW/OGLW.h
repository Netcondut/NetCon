#ifndef OGLW_H_INCLUDED
#define OGLW_H_INCLUDED

#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>

#include "include/Error.h"

#include "include/Texture2D.h"


struct float2
{
    float x,y;
};
struct WindowParameters
{
    int width, height;
    bool depth;
};
struct QuadDesc
{
  float width,height;
  float2 pos;
};
class OGLW
{
private:
    Display   *dpy;
    Window     win;
    bool dblbuf;

public:
    OGLW(int argc, char **argv, WindowParameters params);
    ~OGLW();
    void ParamDevice(WindowParameters params);
    void SwapBuffers();
    Display* GetDisplay();
    Window GetWindow();
    bool dblBuffer();
};
class Quad
{
    private:
    QuadDesc desc;
    Texture2D* tex;
    public:

    Quad(QuadDesc _desc);
    ~Quad();
    void SetCallList(int ID);

};


#endif // OGLW_H_INCLUDED

