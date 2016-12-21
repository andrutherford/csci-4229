/*
* Simple program to demonstrate generating coordinates
* using the Lorenz Attractor
*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Global variables
int th = 0;       // Azimuth
int ph = 0;       // Elevation
double z = 0;     // Z
double d = 2;   // Dimension
double w = 0.02;

/*  Lorenz Parameters  */
double s  = 10;
double b  = 2.6666;
double r  = 28;

int N = 50000;
double pa[50000][3];
double color[50000][3];

#define LEN 8192  // Maximum length
void Print(const char* format , ...)
{
  char    buf[LEN];
  char*   ch=buf;
  va_list args;
  //  Turn the parameters into a character string
  va_start(args,format);
  vsnprintf(buf,LEN,format,args);
  va_end(args);
  //  Display the characters one at a time at the current raster position
  while (*ch)
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

void lorenz_gen(void)
{
  double x = 1;
  double y = 1;
  double z = 1;

  double dx = 0;
  double dy = 0;
  double dz = 0;
  int i = 0;
  /*  Time step  */
  double dt = 0.001;
  /*
  *  Integrate 50,000 steps (50 time units with dt = 0.001)
  *  Explicit Euler integration
  */
  for (i = 0; i < N; i++)
  {
    dx = s * (y - x);
    dy = x * (r - z) - y;
    dz = x * y - b * z;
    x += dt * dx;
    y += dt * dy;
    z += dt * dz;
    pa[i][0] = x * w;
    pa[i][1] = y * w;
    pa[i][2] = z * w;
  }
}

/*
*  Display the scene
*/
void display()
{
  lorenz_gen();
  //  Clear the image
  glClear(GL_COLOR_BUFFER_BIT);
  //  Reset previous transforms
  glLoadIdentity();
  //  Set view angle
  glRotated(ph,1,0,0);
  glRotated(th,0,1,0);
  int i;
  glBegin(GL_LINE_STRIP);
  for (i = 0;i < N;i++)
  {
    glColor3dv(pa[i]);
    glVertex3dv(pa[i]);
  }
  glEnd();
  //  Draw axes in white
  glColor3f(1,1,1);
  glBegin(GL_LINES);
  glVertex3d(0,0,0);
  glVertex3d(1,0,0);
  glVertex3d(0,0,0);
  glVertex3d(0,1,0);
  glVertex3d(0,0,0);
  glVertex3d(0,0,1);
  glEnd();
  //  Label axes
  glRasterPos3d(1,0,0);
  Print("X");
  glRasterPos3d(0,1,0);
  Print("Y");
  glRasterPos3d(0,0,1);
  Print("Z");
  //  Display parameters
  glWindowPos2i(5,5);
  Print("View Angle=%d,%d",th,ph);

  //  Flush and swap
  glFlush();
  glutSwapBuffers();
}

void key(unsigned char k, int x, int y) {
  if (k == 27)
  exit(0);
  else if (k == '0')
  {
    th = 0;
    ph = 0;
  }
  else if (k == '1')
  {
    s = 10;
    b = 2.666;
    r = 28;
  }
  else if (k=='a')
  s -= 1; //sigma down
  else if (k=='q')
  s += 1; //sigma up
  else if (k=='s')
  r -= 0.5; //rho down
  else if (k=='w')
  r += 0.5; //rho up
  else if (k=='d')
  b -= 1;   //beta down
  else if (k=='e')
  b += 1;   //beta up
  glutPostRedisplay();
}


/*
*  GLUT calls this routine when the window is resized
*/
void reshape(int width,int height)
{
  //  Ratio of the width to the height of the window
  double w2h = (height > 0) ? (double)width / height : 1;
  //  Set the viewport to the entire window
  glViewport(0,0, width,height);
  //  Tell OpenGL we want to manipulate the projection matrix
  glMatrixMode(GL_PROJECTION);
  //  Undo previous transformations
  glLoadIdentity();
  //  Orthogonal projection box adjusted for the
  //  aspect ratio of the window
  glOrtho(-d * w2h,+d * w2h, -d,+d, -d,+d);
  //  Switch to manipulating the model matrix
  glMatrixMode(GL_MODELVIEW);
  //  Undo previous transformations
  glLoadIdentity();
}

/*
*  GLUT calls this routine when an arrow key is pressed
*/
void special(int key,int x,int y)
{
  //  Right arrow key - increase azimuth by 5 degrees
  if (key == GLUT_KEY_RIGHT)
  th += 5;
  //  Left arrow key - decrease azimuth by 5 degrees
  else if (key == GLUT_KEY_LEFT)
  th -= 5;
  //  Up arrow key - increase elevation by 5 degrees
  else if (key == GLUT_KEY_UP)
  ph += 5;
  //  Down arrow key - decrease elevation by 5 degrees
  else if (key == GLUT_KEY_DOWN)
  ph -= 5;
  //  Keep angles to +/-360 degrees
  th %= 360;
  ph %= 360;
  //  Tell GLUT it is necessary to redisplay the scene
  glutPostRedisplay();
}


// Main
int main(int argc,char* argv[])
{
  //  Initialize GLUT and process user parameters
  glutInit(&argc,argv);
  //  Request double buffered, true color window
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowPosition(50, 50);
  //  Request 500 x 500 pixel window
  glutInitWindowSize(500,500);
  //  Create the window
  glutCreateWindow("Andrew Rutherford - Homework 2: Lorenz Attractor");
  //  Tell GLUT to call "display" when the scene should be drawn
  glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
  glutReshapeFunc(reshape);
  //  Tell GLUT to call "special" when an arrow key is pressed
  glutSpecialFunc(special);
  //  Tell GLUT to call "key" when a key is pressed
  glutKeyboardFunc(key);
  //  Tell GLUT to call "key" when a key is pressed
  glutMainLoop();
  //  Return code
  return 0;
}
