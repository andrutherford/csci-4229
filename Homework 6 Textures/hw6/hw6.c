/*
 * Andrew Rutherford
 * CSCI 4229
 * Homework 6


 *  Key bindings:
 *  l          Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  F2         Toggle local viewer mode
 *  F3         Toggle light distance (1/5)
 *  F8         Change ball increment
 *  F9         Invert bottom normal
 *  m          Toggles light movement
 *  []         Lower/rise light
 *  p          Toggles ortogonal/perspective projection
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */


 #include "CSCIx229.h"

 int axes=1;       //  Display axes
 int mode=1;       //  Projection mode
 int move=1;       //  Move light
 int th=0;         //  Azimuth of view angle
 int ph=10;         //  Elevation of view angle
 int fov=55;       //  Field of view (for perspective)
 int light=1;      //  Lighting
 double asp=1;     //  Aspect ratio
 double dim=3.0;   //  Size of world
 // Light values
 int one       =   1;  // Unit value
 int distance  =   5;  // Light distance
 int inc       =  10;  // Ball increment
 int smooth    =   1;  // Smooth/Flat shading
 int local     =   0;  // Local Viewer Model
 int emission  =   0;  // Emission intensity (%)
 int ambient   =  30;  // Ambient intensity (%)
 int diffuse   = 100;  // Diffuse intensity (%)
 int specular  =   0;  // Specular intensity (%)
 int shininess =   0;  // Shininess (power of two)
 float shinyvec[1];    // Shininess (value)
 int zh        =  90;  // Light azimuth
 float ylight  =   0;  // Elevation of light

 //Textures
 unsigned int texture[10];

 /*
  *  Draw vertex in polar coordinates with normal
  */
 static void Vertex(double th,double ph)
 {
    double x = Sin(th)*Cos(ph);
    double y = Cos(th)*Cos(ph);
    double z =         Sin(ph);
    //  For a sphere at the origin, the position
    //  and normal vectors are the same
    glNormal3d(x,y,z);
    glVertex3d(x,y,z);
 }

 /*
  *  Draw a ball
  *     at (x,y,z)
  *     radius (r)
  */
 void ball(double x,double y,double z,double r)
 {
    int th,ph;
    float yellow[] = {1.0,1.0,0.0,1.0};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glScaled(r,r,r);
    //  White ball
    glColor3f(1,1,1);
    glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
    glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
    //  Bands of latitude
    for (ph=-90;ph<90;ph+=inc)
    {
       glBegin(GL_QUAD_STRIP);
       for (th=0;th<=360;th+=2*inc)
       {
          Vertex(th,ph);
          Vertex(th,ph+inc);
       }
       glEnd();
    }
    //  Undo transformations
    glPopMatrix();
 }

static void house(double x, double y, double z,
		      double dx, double dy, double dz,
		      double th){

  const double roof_top = 2; //height of roof
  const double roofoff = 1.5;
  const double roof_width = 1.25; //how long roof is
  const double roof_bottom = .75; //what height roof begins

  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);


  glPushMatrix();

  // Translations
  glTranslated(x, y, z);
  glRotated(th, 0, 1, 0);
  glScaled(dx, dy, dz);

  //Textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  // Body of house
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glBegin(GL_QUADS);
  //Front wall
  //glColor3ub(113,100,0);
  glNormal3f( 0, 0, 1);
  glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1, 1);
  glTexCoord2f(5.0, 0.0); glVertex3f(+1,-1, 1);
  glTexCoord2f(5.0, 5.0); glVertex3f(+1,+1, 1);
  glTexCoord2f(0.0, 5.0); glVertex3f(-1,+1, 1);
  //  Back wall
  //glColor3ub(153,130,0);
  glNormal3f( 0, 0,-1);
  glTexCoord2f(0.0, 0.0); glVertex3f(+1,-1,-1);
  glTexCoord2f(5.0, 0.0); glVertex3f(-1,-1,-1);
  glTexCoord2f(5.0, 5.0); glVertex3f(-1,+1,-1);
  glTexCoord2f(0.0, 5.0); glVertex3f(+1,+1,-1);

  //  Right wall
  //glColor3ub(153,90,0);
  glNormal3f(+1, 0, 0);
  glTexCoord2f(0.0, 0.0); glVertex3f(+1,-1,+1);
  glTexCoord2f(5.0, 0.0); glVertex3f(+1,-1,-1);
  glTexCoord2f(5.0, 5.0); glVertex3f(+1,+1,-1);
  glTexCoord2f(0.0, 5.0); glVertex3f(+1,+1,+1);
  //  Left wall
  //glColor3ub(153,90,0);
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1,-1);
  glTexCoord2f(5.0, 0.0); glVertex3f(-1,-1,+1);
  glTexCoord2f(5.0, 5.0); glVertex3f(-1,+1,+1);
  glTexCoord2f(0.0, 5.0); glVertex3f(-1,+1,-1);
  //  Top wall
  //glColor3ub(153,0,100);
  glNormal3f( 0,+1, 0);
  glVertex3f(-1,+1,+1);
  glVertex3f(+1,+1,+1);
  glVertex3f(+1,+1,-1);
  glVertex3f(-1,+1,-1);
  //  Bottom wall
  //glColor3ub(153,0,100);
  glNormal3f( 0,-1, 0);
  glVertex3f(-1,-1,-1);
  glVertex3f(+1,-1,-1);
  glVertex3f(+1,-1,+1);
  glVertex3f(-1,-1,+1);
  glEnd();

  // Roof
  glBindTexture(GL_TEXTURE_2D, texture[2]);
  glBegin(GL_TRIANGLES);
  // Front face
  //glColor3ub(30,30,30);
  glNormal3f(0,0,+1);
  glTexCoord2f(0.0, 0.0); glVertex3f(+roofoff,+roof_bottom,+roof_width);
  glTexCoord2f(2.0, 0.0); glVertex3f(-roofoff,+roof_bottom,+roof_width);
  glTexCoord2f(1.0, 2.0); glVertex3f(0,+roof_top,+roof_width);
  // Back face
  glNormal3f(0,0,-1);
  glTexCoord2f(0.0, 0.0); glVertex3f(+roofoff,+roof_bottom,-roof_width);
  glTexCoord2f(2.0, 0.0); glVertex3f(-roofoff,+roof_bottom,-roof_width);
  glTexCoord2f(1.0, 2.0); glVertex3f(0,+roof_top,-roof_width);
  glEnd();
  // Roof sides
  glBegin(GL_QUADS);
  // Right side
  //glColor3ub(50,50,50);
  glNormal3f(+1,+1,0 );
  glTexCoord2f(0.0, 0.0); glVertex3f(+roofoff,+roof_bottom,-roof_width);
  glTexCoord2f(2.0, 0.0); glVertex3f(+roofoff,+roof_bottom,+roof_width);
  glTexCoord2f(2.0, 2.0); glVertex3f(0,+roof_top,+roof_width);
  glTexCoord2f(0.0, 2.0); glVertex3f(0,+roof_top,-roof_width);
  // Left Side
  //glColor3ub(90,90,90);
  glNormal3f(-1,-1,0);
  glTexCoord2f(0.0, 0.0); glVertex3f(-roofoff,+roof_bottom,-roof_width);
  glTexCoord2f(2.0, 0.0); glVertex3f(-roofoff,+roof_bottom,+roof_width);
  glTexCoord2f(2.0, 2.0); glVertex3f(0,+roof_top,+roof_width);
  glTexCoord2f(0.0, 2.0); glVertex3f(0,+roof_top,-roof_width);
  // Bottom
  //glColor3ub(153,51,0);
  glNormal3f(0,+1,0);
  glVertex3f(+roofoff,+roof_bottom,-roof_width);
  glVertex3f(+roofoff,+roof_bottom,+roof_width);
  glVertex3f(-roofoff,+roof_bottom,+roof_width);
  glVertex3f(-roofoff,+roof_bottom,-roof_width);
  glEnd();

  // Chimney
  glBindTexture(GL_TEXTURE_2D, texture[5]);
  glBegin(GL_QUADS);
  // Left face
  //glColor3ub(70,0,0);
  glNormal3f( 1, 0,   0);
  glTexCoord2f(0.0, 0.0); glVertex3f(-.5,+1,+.25);
  glTexCoord2f(2.0, 0.0); glVertex3f(-.5,+1,-.25);
  glTexCoord2f(2.0, 2.0); glVertex3f(-.5,+2,-.25);
  glTexCoord2f(0.0, 2.0); glVertex3f(-.5,+2,+.25);
  // Right face
  //glColor3ub(50,0,0);
  glNormal3f(-1, 0,   0);
  glTexCoord2f(0.0, 0.0); glVertex3f(-1,+1,+.25);
  glTexCoord2f(2.0, 0.0); glVertex3f(-1,+1,-.25);
  glTexCoord2f(2.0, 2.0); glVertex3f(-1,+2,-.25);
  glTexCoord2f(0.0, 2.0); glVertex3f(-1,+2,+.25);
  // Back face
  //glColor3ub(110,0,0);
  glNormal3f(  0, 0,  -1);
  glTexCoord2f(0.0, 0.0); glVertex3f(-.5,+1,-.25);
  glTexCoord2f(2.0, 0.0); glVertex3f(-.5,+2,-.25);
  glTexCoord2f(2.0, 2.0); glVertex3f(-1,+2,-.25);
  glTexCoord2f(0.0, 2.0); glVertex3f(-1,+1,-.25);
  // Front face
  //glColor3ub(110,0,0);
  glNormal3f(  0, 0,  +1);
  glTexCoord2f(0.0, 0.0); glVertex3f(-.5,+1,+.25);
  glTexCoord2f(2.0, 0.0); glVertex3f(-.5,+2,+.25);
  glTexCoord2f(2.0, 2.0); glVertex3f(-1,+2,+.25);
  glTexCoord2f(0.0, 2.0); glVertex3f(-1,+1,+.25);
  // Top face
  //glColor3ub(10,10,10);
  glTexCoord2f(0.0, 0.0); glVertex3f(-.5,+2,+.25);
  glTexCoord2f(2.0, 0.0); glVertex3f(-.5,+2,-.25);
  glTexCoord2f(2.0, 2.0); glVertex3f(-1,+2,-.25);
  glTexCoord2f(0.0, 2.0); glVertex3f(-1,+2,+.25);
  glEnd();

  // Door
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(-1, -1);
  glBindTexture(GL_TEXTURE_2D, texture[3]); // Door
  glBegin(GL_QUADS);
  // Front face
  glNormal3f(  0, 0, -1);
  glTexCoord2f(0.0, 0.0); glVertex3f(+.2,-1,-1.001);
  glTexCoord2f(1.0, 0.0); glVertex3f(-.2,-1,-1.001);
  glTexCoord2f(1.0, 1.0); glVertex3f(-.2,0,-1.001);
  glTexCoord2f(0.0, 1.0); glVertex3f(+.2,0,-1.001);
  glEnd();

  // Draw Sidewalk
  glEnable(GL_POLYGON_OFFSET_FILL);
  glBindTexture(GL_TEXTURE_2D,texture[5]);
  glBegin(GL_QUADS);

  // Top
  glNormal3f(  0, +1,0);
  glTexCoord2f(0.0, 0.0); glVertex3f(+.2,-.99,-3);
  glTexCoord2f(4.0, 0.0); glVertex3f(+.2,-.99,-1.0);
  glTexCoord2f(4.0, 4.0); glVertex3f(-.2,-.99,-1.0);
  glTexCoord2f(0.0, 4.0); glVertex3f(-.2,-.99,-3);

  glEnd();

  // Draw right window
  glBindTexture(GL_TEXTURE_2D, texture[4]); // Window
  glBegin(GL_QUADS);
  // Front
  glNormal3f(  0, 0, -1);
  glTexCoord2f(0.0, 0.0); glVertex3f(+.8,-.5,-1.001);
  glTexCoord2f(.75, 0.0); glVertex3f(+.4,-.5,-1.001);
  glTexCoord2f(.75, 1.0); glVertex3f(+.4,0,-1.001);
  glTexCoord2f(0.0, 1.0); glVertex3f(+.8,0,-1.001);
  glEnd();

  // Draw left window
  glBindTexture(GL_TEXTURE_2D, texture[4]); // Window
  glBegin(GL_QUADS);
  // Front
  glNormal3f(  0, 0, -1);
  glTexCoord2f(0.0, 0.0); glVertex3f(-.8,-.5,-1.001);
  glTexCoord2f(.75, 0.0); glVertex3f(-.4,-.5,-1.001);
  glTexCoord2f(.75, 1.0); glVertex3f(-.4,0,-1.001);
  glTexCoord2f(0.0, 1.0); glVertex3f(-.8,0,-1.001);
  glEnd();


  glPopMatrix();
}

static void drawScene(){

  house(-6,1,2 , 1,1,1 , 0);
  house(-6,1,-6 , 1.5,1,1, 270);
  house(2,1.2,2 , 1,1.2,1 , 0);
  house(2,1,-6 , 1,1,1.5, 90);

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  //Ground/grass
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  glBegin(GL_QUADS);
  glNormal3f(0, 1, 0);
  glTexCoord2f(0.0,  0.0); glVertex3f(-100,0,-100);
  glTexCoord2f(200.0,0.0); glVertex3f(-100,0,100);
  glTexCoord2f(200.0,200.0); glVertex3f(100,0,100);
  glTexCoord2f(0.0,  200.0); glVertex3f(100,0,-100);
  glEnd();

  //Road
  glBindTexture(GL_TEXTURE_2D,texture[8]);
  glBegin(GL_QUADS);
  glTexCoord2f(4.0, 0.0); glVertex3f(-50,.05,-1);
  glTexCoord2f(4.0, 1.0); glVertex3f(-50,.05,-3);
  glTexCoord2f(0.0, 1.0); glVertex3f(50, .05,-3);
  glTexCoord2f(0.0, 0.0); glVertex3f(50, .05,-1);
  glEnd();

  //Road
  glBindTexture(GL_TEXTURE_2D,texture[8]);
  glBegin(GL_QUADS);
  glTexCoord2f(4.0, 0.0); glVertex3f(-1,.05,-50);
  glTexCoord2f(4.0, 1.0); glVertex3f(-3,.05,-50);
  glTexCoord2f(0.0, 1.0); glVertex3f(-3,.05,50);
  glTexCoord2f(0.0, 0.0); glVertex3f(-1,.05,50);
  glEnd();

  //Road markers
  int i;
  for (i=-50; i <=50; i++)
  {
    glColor3ub(255,255,0);
    glBegin(GL_QUADS);
    glVertex3f(i-.15,.06,-1.9);
    glVertex3f(i-.15,.06,-2.1);
    glVertex3f(i+.15,.06,-1.9);
    glVertex3f(i+.15,.06,-2.1);
    glEnd();
  }
  for (i=-50; i <=50; i++)
  {
    glColor3ub(255,255,0);
    glBegin(GL_QUADS);
    glVertex3f(-1.9,.06,i-.15);
    glVertex3f(-2.1,.06,i-.15);
    glVertex3f(-1.9,.06,i+.15);
    glVertex3f(-2.1,.06,i+.15);
    glEnd();
  }

  glColor3ub(0,30,0);
  glBegin(GL_QUADS);
  glNormal3f(0, 1, 0);
  glVertex3f(-100,0,-100);
  glVertex3f(-100,0,100);
  glVertex3f(100,0,100);
  glVertex3f(100,0,-100);
  glEnd();


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

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);






   drawScene();

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
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

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perspective":"Orthogonal",light?"On":"Off");
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shinyvec[0]);
   }

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}


/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th -= 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th += 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Smooth color model
   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   else if (key == GLUT_KEY_F3)
      distance = (distance==1) ? 5 : 1;
   //  Toggle ball increment
   else if (key == GLUT_KEY_F8)
      inc = (inc==10)?3:10;
   //  Flip sign
   else if (key == GLUT_KEY_F9)
      one = -one;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(mode?fov:0,asp,dim);
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
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Translate shininess power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode?fov:0,asp,dim);
   //  Animate if requested
   //glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
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
   Project(mode?fov:0,asp,dim);
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
   glutCreateWindow("Andrew Rutherford - Homework 6");
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   texture[0] = LoadTexBMP("grass.bmp");
   texture[1] = LoadTexBMP("wall.bmp");
   texture[2] = LoadTexBMP("roof.bmp");
   texture[3] = LoadTexBMP("door.bmp");
   texture[4] = LoadTexBMP("window.bmp");
   texture[5] = LoadTexBMP("sidewalk.bmp");

   ErrCheck("init");
   glutMainLoop();
   return 0;
}
