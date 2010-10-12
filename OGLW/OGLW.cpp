#include "OGLW.h"

OGLW::OGLW(int argc, char **argv, WindowParameters params)
{
      XVisualInfo         *vi;
      Colormap             cmap;
      XSetWindowAttributes swa;
      GLXContext           cx;
      dblbuf = GL_TRUE;
      int                  dummy;
      static int snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
      static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};
      /*** (1) open a connection to the X server ***/

      dpy = XOpenDisplay(NULL);
      if (dpy == NULL)
        fatalError("could not open display");

      /*** (2) make sure OpenGL's GLX extension supported ***/

      if(!glXQueryExtension(dpy, &dummy, &dummy))
        fatalError("X server has no OpenGL GLX extension");

      /*** (3) find an appropriate visual ***/

      /* find an OpenGL-capable RGB visual with depth buffer */
      vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
      if (vi == NULL)
      {
        vi = glXChooseVisual(dpy, DefaultScreen(dpy), snglBuf);
        if (vi == NULL) fatalError("no RGB visual with depth buffer");
        dblbuf = GL_FALSE;
      }
      if(vi->c_class != TrueColor)
        fatalError("TrueColor visual required for this program");

      /*** (4) create an OpenGL rendering context  ***/

      /* create an OpenGL rendering context */
      cx = glXCreateContext(dpy, vi, /* no shared dlists */ None,
                            /* direct rendering if possible */ GL_TRUE);
      if (cx == NULL)
        fatalError("could not create rendering context");

      /*** (5) create an X window with the selected visual ***/

      /* create an X colormap since probably not using default visual */
      cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
      swa.colormap = cmap;
      swa.border_pixel = 0;
      swa.event_mask = KeyPressMask    | ExposureMask
                     | ButtonPressMask | StructureNotifyMask;
      win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0,
                          params.width, params.height, 0, vi->depth, InputOutput, vi->visual,
                          CWBorderPixel | CWColormap | CWEventMask, &swa);
      XSetStandardProperties(dpy, win, "main", "main", None,
                             argv, argc, NULL);

      /*** (6) bind the rendering context to the window ***/

      glXMakeCurrent(dpy, win, cx);

      /*** (7) request the X window to be displayed on the screen ***/

      XMapWindow(dpy, win);

      if(params.depth)
      {
        glEnable(GL_DEPTH_TEST); /* enable depth buffering */
        glDepthFunc(GL_LESS);    /* pedantic, GL_LESS is the default */
      }
      else
        glDisable(GL_DEPTH_TEST);

}
OGLW::~OGLW()
{

}
void OGLW::ParamDevice(WindowParameters params)
{

    if(params.depth)
    {
      glEnable(GL_DEPTH_TEST); /* enable depth buffering */
      glDepthFunc(GL_LESS);    /* pedantic, GL_LESS is the default */
    }
    else
      glDisable(GL_DEPTH_TEST);

}
void OGLW::SwapBuffers()
{

    if (dblbuf)
    glXSwapBuffers(dpy, win);/* buffer swap does implicit glFlush */
  else
    glFlush();  /* explicit flush for single buffered case */

}
Display* OGLW::GetDisplay()
{
    return dpy;
}
Window OGLW::GetWindow()
{

    return win;
}
bool OGLW::dblBuffer()
{
    return dblbuf;
}
Quad::Quad(QuadDesc _desc)
{
    desc = _desc;
    tex = new Texture2D("test.tga",256,256,0);
}
Quad::~Quad()
{
    tex->~Texture2D();
}
void Quad::SetCallList(int ID)
{
    /* otherwise compile and execute to create the display list */
    glNewList(ID, GL_COMPILE_AND_EXECUTE);

    /* front face */
    glBegin(GL_QUADS);
      glColor3f(0.7, 0.7, 0.1);  /* green */
      glVertex3f(0.0,         0.0,        0.0);
      glVertex3f(desc.height, 0.0,        0.0);
      glVertex3f(desc.height, desc.width, 0.0);
      glVertex3f(0.0,         desc.width, 0.0);

    glEnd();
    glEndList();
}
