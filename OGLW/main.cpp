/* A simple program to show how to set up an X window for OpenGL rendering.
 * X86 compilation: gcc -o -L/usr/X11/lib   main main.c -lGL -lX11
 * X64 compilation: gcc -o -L/usr/X11/lib64 main main.c -lGL -lX11
 */
#include <stdio.h>
#include <stdlib.h>

#include "OGLW.h"
#include "include/Error.h"


void redraw(OGLW* WIN)
{
  static GLboolean   displayListInited = GL_FALSE;

      /* frame buffer clears should be to black */
      glClearColor(0.0, 0.0, 0.0, 0.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* if display list already exists, just execute it */
    glCallList(1);

  WIN->SwapBuffers();
}

int main(int argc, char **argv)
{

  WindowParameters params;
  params.depth = false;
  params.width = 800;
  params.height = 800;

  OGLW* window = new OGLW(argc,argv, params);
  glOrtho(0.0f,1.0f,0.0f,1.0f,-1.0f,1.0f);

  XEvent               event;
  GLboolean            needRedraw = GL_FALSE, recalcModelView = GL_TRUE;
  /*** (8) configure the OpenGL context for rendering ***/



 // glClearDepth(1.0);       /* pedantic, 1.0 is the default */
    Quad* quad;
    QuadDesc desc;
    desc.height = 1.0f;
    desc.width = 1.0f;
    desc.pos.x = 0.0f;
    desc.pos.y = 0.0f;
    quad = new Quad(desc);
    quad->SetCallList(1);

  /* set up projection transform */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);
  /* establish initial viewport */
  /* pedantic, full window size is default viewport */
  //glViewport(0, 0, 300, 300);

 /* printf( "Press left mouse button to rotate around X axis\n" );
  printf( "Press middle mouse button to rotate around Y axis\n" );
  printf( "Press right mouse button to rotate around Z axis\n" );
  printf( "Press ESC to quit the application\n" );*/

  /*** (9) dispatch X events ***/

  while (1)
  {
      glMatrixMode(GL_MODELVIEW);

      /* reset modelview matrix to the identity matrix */
      glLoadIdentity();
       redraw(window);
    do
    {
      XNextEvent(window->GetDisplay(), &event);
      switch (event.type)
      {
        case KeyPress:
        {
          KeySym     keysym;
          XKeyEvent *kevent;
          char       buffer[1];
          /* It is necessary to convert the keycode to a
           * keysym before checking if it is an escape */
          kevent = (XKeyEvent *) &event;
          if (   (XLookupString((XKeyEvent *)&event,buffer,1,&keysym,NULL) == 1)
              && (keysym == (KeySym)XK_Escape) )
            exit(0);
          break;
        }
        case ButtonPress:
          recalcModelView = GL_TRUE;
          switch (event.xbutton.button)
          {

          }
          break;
        case ConfigureNotify:
          glViewport(0, 0, event.xconfigure.width,
                     event.xconfigure.height);
      }
    } while(XPending(window->GetDisplay())); /* loop to compress events */

    if (recalcModelView)
    {


      recalcModelView = GL_FALSE;
      needRedraw = GL_TRUE;
    }



  }

  return 0;
}
