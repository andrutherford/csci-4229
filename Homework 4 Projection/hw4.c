/*
 * Andrew Rutherford
 * CSCI 4229
 * Homework 4
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle=0.0;
float lastx, lasty;
int th=165;         //  Azimuth of view angle
int ph=30;         //  Elevation of view angle
double zh=0;      //  Rotation of teapot
int axes=0;       //  Display axes
int mode=0;       //  What to display
int FPS=60;
int fov=55;
double asp=1;
double dim=9.0;

//  Macro for sin & cos in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
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

/*
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode)
      gluPerspective(fov,asp,dim/10,10*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

static void house(double x, double y, double z,
		      double dx, double dy, double dz,
		      double th){

  const double roof_top = 2; //height of roof
  const double roofoff = 1.5;
  const double roof_width = 1.25; //how long roof is
  const double roof_bottom = .75; //what height roof begins
  glPushMatrix();

  // Translations
  glTranslated(x, y, z);
  glRotated(th, 0, 1, 0);
  glScaled(dx, dy, dz);

  // Body of house
  glBegin(GL_QUADS);
  //Front wall
  glColor3ub(113,100,0);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  //  Back wall
  glColor3ub(153,130,0);
  glVertex3f(+1,-1,-1);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,+1,-1);
  glVertex3f(+1,+1,-1);
  //  Right wall
  glColor3ub(153,90,0);
  glVertex3f(+1,-1,+1);
  glVertex3f(+1,-1,-1);
  glVertex3f(+1,+1,-1);
  glVertex3f(+1,+1,+1);
  //  Left wall
  glColor3ub(153,90,0);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,-1,+1);
  glVertex3f(-1,+1,+1);
  glVertex3f(-1,+1,-1);
  //  Top wall
  glColor3ub(153,0,100);
  glVertex3f(-1,+1,+1);
  glVertex3f(+1,+1,+1);
  glVertex3f(+1,+1,-1);
  glVertex3f(-1,+1,-1);
  //  Bottom wall
  glColor3ub(153,0,100);
  glVertex3f(-1,-1,-1);
  glVertex3f(+1,-1,-1);
  glVertex3f(+1,-1,+1);
  glVertex3f(-1,-1,+1);
  glEnd();

  // Roof
  glBegin(GL_TRIANGLES);
  // Front face
  glColor3ub(30,30,30);
  glVertex3f(+roofoff,+roof_bottom,+roof_width);
  glVertex3f(-roofoff,+roof_bottom,+roof_width);
  glVertex3f(0,+roof_top,+roof_width);
  // Back face
  glVertex3f(+roofoff,+roof_bottom,-roof_width);
  glVertex3f(-roofoff,+roof_bottom,-roof_width);
  glVertex3f(0,+roof_top,-roof_width);
  glEnd();
  // Roof sides
  glBegin(GL_QUADS);
  // Right side
  glColor3ub(50,50,50);
  glVertex3f(+roofoff,+roof_bottom,-roof_width);
  glVertex3f(+roofoff,+roof_bottom,+roof_width);
  glVertex3f(0,+roof_top,+roof_width);
  glVertex3f(0,+roof_top,-roof_width);
  // Left Side
  glColor3ub(90,90,90);
  glVertex3f(-roofoff,+roof_bottom,-roof_width);
  glVertex3f(-roofoff,+roof_bottom,+roof_width);
  glVertex3f(0,+roof_top,+roof_width);
  glVertex3f(0,+roof_top,-roof_width);
  // Bottom
  glColor3ub(153,51,0);
  glVertex3f(+roofoff,+roof_bottom,-roof_width);
  glVertex3f(+roofoff,+roof_bottom,+roof_width);
  glVertex3f(-roofoff,+roof_bottom,+roof_width);
  glVertex3f(-roofoff,+roof_bottom,-roof_width);
  glEnd();

  // Chimney
  glBegin(GL_QUADS);
  // Left face
  glColor3ub(70,0,0);
  glVertex3f(-.5,+1,+.25);
  glVertex3f(-.5,+1,-.25);
  glVertex3f(-.5,+2,-.25);
  glVertex3f(-.5,+2,+.25);
  // Right face
  glColor3ub(50,0,0);
  glVertex3f(-1,+1,+.25);
  glVertex3f(-1,+1,-.25);
  glVertex3f(-1,+2,-.25);
  glVertex3f(-1,+2,+.25);
  // Back face
  glColor3ub(110,0,0);
  glVertex3f(-.5,+1,-.25);
  glVertex3f(-.5,+2,-.25);
  glVertex3f(-1,+2,-.25);
  glVertex3f(-1,+1,-.25);
  // Front face
  glColor3ub(110,0,0);
  glVertex3f(-.5,+1,+.25);
  glVertex3f(-.5,+2,+.25);
  glVertex3f(-1,+2,+.25);
  glVertex3f(-1,+1,+.25);
  // Top face
  glColor3ub(10,10,10);
  glVertex3f(-.5,+2,+.25);
  glVertex3f(-.5,+2,-.25);
  glVertex3f(-1,+2,-.25);
  glVertex3f(-1,+2,+.25);
  glEnd();

  // Door
  glBegin(GL_QUADS);
  glColor3ub(100,50,0);
  // Front face
  glVertex3f(+.2,-1,-1.1);
  glVertex3f(-.2,-1,-1.1);
  glVertex3f(-.2,0,-1.1);
  glVertex3f(+.2,0,-1.1);
  // Top face
  glVertex3f(+.2,0,-1.1);
  glVertex3f(+.2,0,-1.0);
  glVertex3f(-.2,0,-1.0);
  glVertex3f(-.2,0,-1.1);
  // Right face
  glVertex3f(+.2,0,-1.1);
  glVertex3f(+.2,-1,-1.1);
  glVertex3f(+.2,-1,-1.0);
  glVertex3f(+.2,0,-1.0);
  // Left face
  glVertex3f(-.2,0,-1.1);
  glVertex3f(-.2,-1,-1.1);
  glVertex3f(-.2,-1,-1.0);
  glVertex3f(-.2,0,-1.0);
  glEnd();

  // Draw Sidewalk
  glBegin(GL_QUADS);
  glColor3ub(50,50,50);
  // Front
  glVertex3f(+.2,-1,-3);
  glVertex3f(-.2,-1,-3);
  glVertex3f(-.2,-.9,-3);
  glVertex3f(+.2,-.9,-3);
  // Top
  glVertex3f(+.2,-.9,-3);
  glVertex3f(+.2,-.9,-1.0);
  glVertex3f(-.2,-.9,-1.0);
  glVertex3f(-.2,-.9,-3);
  // Right
  glVertex3f(+.2,-.9,-3);
  glVertex3f(+.2,-1,-3);
  glVertex3f(+.2,-1,-1.0);
  glVertex3f(+.2,-.9,-1.0);
  // Left
  glVertex3f(-.2,-.9,-3);
  glVertex3f(-.2,-1,-3);
  glVertex3f(-.2,-1,-1.0);
  glVertex3f(-.2,-.9,-1.0);
  glEnd();

  // Draw right window
  glBegin(GL_QUADS);
  glColor3ub(100,100,100);
  // Front
  glVertex3f(+.8,-.5,-1.1);
  glVertex3f(+.4,-.5,-1.1);
  glVertex3f(+.4,0,-1.1);
  glVertex3f(+.8,0,-1.1);
  // Top
  glVertex3f(+.8,0,-1.1);
  glVertex3f(+.8,0,-1.0);
  glVertex3f(+.4,0,-1.0);
  glVertex3f(+.4,0,-1.1);
  // Right
  glVertex3f(+.8,0,-1.1);
  glVertex3f(+.8,-.5,-1.1);
  glVertex3f(+.8,-.5,-1.0);
  glVertex3f(+.8,0,-1.0);
  // Left
  glVertex3f(+.4,0,-1.1);
  glVertex3f(+.4,-.5,-1.1);
  glVertex3f(+.4,-.5,-1.0);
  glVertex3f(+.4,0,-1.0);
  glEnd();

  // Draw left window
  glBegin(GL_QUADS);
  glColor3ub(100,100,100);
  // Front
  glVertex3f(-.8,-.5,-1.1);
  glVertex3f(-.4,-.5,-1.1);
  glVertex3f(-.4,0,-1.1);
  glVertex3f(-.8,0,-1.1);
  // Top
  glVertex3f(-.8,0,-1.1);
  glVertex3f(-.8,0,-1.0);
  glVertex3f(-.4,0,-1.0);
  glVertex3f(-.4,0,-1.1);
  // Right
  glVertex3f(-.8,0,-1.1);
  glVertex3f(-.8,-.5,-1.1);
  glVertex3f(-.8,-.5,-1.0);
  glVertex3f(-.8,0,-1.0);
  // Left
  glVertex3f(-.4,0,-1.1);
  glVertex3f(-.4,-.5,-1.1);
  glVertex3f(-.4,-.5,-1.0);
  glVertex3f(-.4,0,-1.0);
  glEnd();


  glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=3;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();

   //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }
   house(-6,0,2 , 1,1,1 , 0);
   house(-6,0,-6 , 1.5,1,1, 270);
   house(2,.2,2 , 1,1.2,1 , 0);
   house(2,0,-6 , 1,1,1.5, 90);
   house(6,1,2 , 2,2,2 , 0);


   //Ground/grass
   glColor3ub(0,90,0);
   glBegin(GL_QUADS);
   glVertex3f(-100,-1,-100);
   glVertex3f(-100,-1,100);
   glVertex3f(100,-1,100);
   glVertex3f(100,-1,-100);
   glEnd();

   //Road
   glColor3ub(90,90,90);
   glBegin(GL_QUADS);
   glVertex3f(-50,-.95,-1);
   glVertex3f(-50,-.95,-3);
   glVertex3f(50,-.95,-3);
   glVertex3f(50,-.95,-1);
   glEnd();

   //Road
   glColor3ub(90,90,90);
   glBegin(GL_QUADS);
   glVertex3f(-1,-.95,-50);
   glVertex3f(-3,-.95,-50);
   glVertex3f(-3,-.95,50);
   glVertex3f(-1,-.95,50);
   glEnd();

   //Road markers
   int i;
   for (i=-50; i <=50; i++)
   {
     glColor3ub(255,255,0);
     glBegin(GL_QUADS);
     glVertex3f(i-.15,-.9,-1.9);
     glVertex3f(i-.15,-.9,-2.1);
     glVertex3f(i+.15,-.9,-1.9);
     glVertex3f(i+.15,-.9,-2.1);
     glEnd();
   }
   for (i=-50; i <=50; i++)
   {
     glColor3ub(255,255,0);
     glBegin(GL_QUADS);
     glVertex3f(-1.9,-.9,i-.15);
     glVertex3f(-2.1,-.9,i-.15);
     glVertex3f(-1.9,-.9,i+.15);
     glVertex3f(-2.1,-.9,i+.15);
     glEnd();
   }


   glColor3f(1,1,1);
   //  Draw axes
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Five pixels from the lower left corner of the window
   glWindowPos2i(5,5);
   //  Print the text string
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
   //  Render the scene
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
}


/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
  //  PageUp key - increase dim
  else if (key == GLUT_KEY_PAGE_UP)
     dim += 0.1;
  //  PageDown key - decrease dim
  else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
     dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0'){
      th = 0;
      ph = 5;
   }
   //  Toggle axes
   else if (ch == '1')
      axes = 1-axes;
  else if (ch == '2')
    mode = 1-mode;
  //  Change field of view angle
  else if (ch == '-' && ch>1)
     fov--;
  else if (ch == '+' && ch<179)
     fov++;

  else if (ch=='q')
  {
    xrot += 1;
    if (xrot >360) xrot -= 360;
  }
  if (ch=='z' || ch=='Z')
   {
     xrot -= 1;
     if (xrot < -360) xrot += 360;
   }

   if (ch=='w')
   {
     float xrotrad, yrotrad;
     yrotrad = (yrot / 180 * 3.141592654f);
     xrotrad = (xrot / 180 * 3.141592654f);
     xpos += (sin(yrotrad));
     zpos -= (cos(yrotrad));
     ypos -= (sin(xrotrad));
   }

   if (ch=='s')
   {
     float xrotrad, yrotrad;
     yrotrad = (yrot / 180 * 3.141592654f);
     xrotrad = (xrot / 180 * 3.141592654f);
     xpos -= (sin(yrotrad));
     zpos += (cos(yrotrad));
     ypos += (sin(xrotrad));
   }

   if (ch=='d')
   {
     float yrotrad;
     yrotrad = (yrot / 180 * 3.141592654f);
     xpos += (cos(yrotrad)) * 0.2;
     zpos += (sin(yrotrad)) * 0.2;
   }

   if (ch=='a')
   {
     float yrotrad;
     yrotrad = (yrot / 180 * 3.141592654f);
     xpos -= (cos(yrotrad)) * 0.2;
     zpos -= (sin(yrotrad)) * 0.2;
   }



  Project();
  //  Tell GLUT it is necessary to redisplay the scene
  glutPostRedisplay();
}

void mouseMovement(int x, int y) {
    int diffx=x-lastx; //check the difference between the current x and the last x position
    int diffy=y-lasty; //check the difference between the current y and the last y position
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position
    xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
    yrot += (float) diffx;    //set the xrot to yrot with the addition of the difference in the x position
}




/*
 *  GLUT calls this routine when the window is resized
 */
 void reshape(int width,int height)
{
  //  Ratio of the width to the height of the window
  asp = (height>0) ? (double)width/height : 1;
  //  Set the viewport to the entire window
  glViewport(0,0, width,height);
  //  Set projection
  Project();
}
/*
 *  GLUT calls this routine when there is nothing else to do
 */
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360);
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(600,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutCreateWindow("Andrew Rutherford - Homework 4");
   glutIdleFunc(idle);
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutPassiveMotionFunc(mouseMovement);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutMainLoop();
   return 0;
}
