/*
 *  Used perspective view from example 9
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#ifdef USEGLEW
#include <GL/glew.h>
#endif
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
// Tell Xcode IDE to not gripe about OpenGL deprecation
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#include <GL/glut.h>
#endif
//  Default resolution
//  For Retina displays compile with -DRES=2
#ifndef RES
#define RES 1
#endif
float cameraX = 0;
float cameraZ = -5;
float yaw = 0;
int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int th=30;         //  Azimuth of view angle
int ph=45;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=13;   //  Size of world
double distance = 4;
int zh = 90;
double ylight = 0;
int local = 0;
int obj = 0;
float ambient = 10;
float spec = 10;
float r = 50;
float g = 50;
float b = 50;
int rainbow = 0;
int move = 1;
//  Macro for sin & cos in degrees
#define Cos(th) cos(3.14159265/180*(th))
#define Sin(th) sin(3.14159265/180*(th))

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
 *  Check for OpenGL errors
 */
void ErrCheck(const char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

/*
 *  Print message to stderr and exit
 */
void Fatal(const char* format , ...)
{
   va_list args;
   va_start(args,format);
   vfprintf(stderr,format,args);
   va_end(args);
   exit(1);
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
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
//HERERERERERERE
static void Vertex(double th,double ph)
{
   //glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
    double x = Sin(th)*Cos(ph);
    double y = Cos(th)*Cos(ph);
    double z = Sin(ph);
 
    glNormal3d(x,y,z);
    glVertex3d(x,y,z);
}

/*
 *  Draw a sphere (version 1)
 *     at (x,y,z)
 *     radius (r)
 */
static void sphere1(double x,double y,double z,double r, double r2, double r3)
{
    
   const int d=15;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r2,r3);
    //specular to white
    float white[] = {1,1,1,1};
    float shiny[] = {1,1,1,1};
    float Emission[] = {0.0, 0.0, 0.01, 1.0};
    //float shiny = 1;
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, 1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    glMaterialfv(GL_FRONT, GL_EMISSION, Emission);

   //  South pole cap
   glBegin(GL_TRIANGLE_FAN);

   Vertex(0,-90);
   for (int th=0;th<=360;th+=d)
   {
      Vertex(th,d-90);
   }
   glEnd();

   //  Latitude bands
   for (int ph=d-90;ph<=90-2*d;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   //  North pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,90);
   for (int th=0;th<=360;th+=d)
   {
      Vertex(th,90-d);
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
}

static void tail(double x,double y,double z,double r, double r2, double r3)
{
   const int d=15;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   //glRotated(th, 0, 1, 0);
   glScaled(r,r2,r3);
    
   //  South pole cap
   glBegin(GL_TRIANGLE_FAN);

   Vertex(0,-90);
   for (int th=0;th<=360;th+=d)
   {
       if(th>45&&th<135)glColor3f(1,1,1);
       else if(th>225&&th<315)glColor3f(1,1,1);
       else glColor3f(1, 0.7, 0);
      Vertex(th,d-90);
   }
   glEnd();

   //  Latitude bands
    
   for (int ph=d-90;ph<=90-2*d;ph+=d)
   {
      
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=d)
      {
          if(th>70&&th<110)glColor3f(1,1,1);
          else if(th>250&&th<290)glColor3f(1,1,1);
          else glColor3f(1, 0.55, 0);
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   //  North pole cap
   glBegin(GL_TRIANGLE_FAN);
    glColor3f(1,1,1);
   Vertex(0,90);
   for (int th=0;th<=360;th+=d)
   {
       
       if(th>45&&th<135)glColor3f(1,1,1);
       else if(th>225&&th<315)glColor3f(1,1,1);
       else glColor3f(1, 0.55, 0);
      Vertex(th,90-d);
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
}


static void ears(double x, double y, double z, double side, double os, double scale){
    //used to make the quads that form the ears
    
    glPushMatrix();
    glTranslated(x,y,z);
    glRotated(0,0,1,0);
    glScaled(scale, scale, scale);
    
    glBegin(GL_QUADS);
    glColor3f(1,1,1);
    
    //ears!
    //glColor3f(1,1,1);
    glNormal3f(0,0,1);
    glVertex3f(-1.3 *side,-1*os, 1);
    glVertex3f(+0.7*side,-1*os, 1);
    glVertex3f(+1.3*side,+1*os, 1);
    glVertex3f(-0.7*side,+1*os, 1);
    //  Back
    
    //glColor3f(0.5,0.5,0.5);
    glNormal3f(70*side, 10*os, -28);
    glVertex3f(+0.5*side,-1*os,-1);
    glVertex3f(-0.3*side,-1*os,-1);
    glVertex3f(-0.3*side,+0.4*os,-0.5);
    glVertex3f(+0.5*side,+0.4*os,-0.5);
    
    //  Right
    //glColor3f(1,1,0);
    //glColor3f(0.7,0.7,0.7);
    glNormal3f(140*side, 5*os, -14);
    glVertex3f(+0.7*side,-1*os,+1);
    glVertex3f(+0.5*side,-1*os,-1);
    glVertex3f(+0.5*side,+0.4*os,-0.5);
    glVertex3f(+1.3*side,+1*os,+1);
    
    //  Left
    //glColor3f(0,1,0);
    glNormal3f(-10*side, -3*os, 5);
    glVertex3f(-0.3*side,-1*os,-1);
    glVertex3f(-1.3*side,-1*os,+1);
    glVertex3f(-0.7*side,+1*os,+1);
    glVertex3f(-0.3*side,+0.4*os,-0.5);
    
    //  Top
    //glColor3f(0,1,1);
    glNormal3f(0, 15*os, -6);
    glVertex3f(-0.7*side,+1*os,+1);
    glVertex3f(+1.3*side,+1*os,+1);
    glVertex3f(+0.5*side,+0.4*os,-0.5);
    glVertex3f(-0.3*side,+0.4*os,-0.5);
    
    
    //  Bottom
    //glColor3f(1,0,1);
    glNormal3f(0,-1*os,0);
    glVertex3f(-0.3*side,-1*os,-1);
    glVertex3f(+0.5*side,-1*os,-1);
    glVertex3f(+0.7*side,-1*os,+1);
    glVertex3f(-1.3*side,-1*os,+1);
    

    glEnd();
    glPopMatrix();
    
    
    
}
static void leaf(double x, double y, double z, double s, double r, double r2, double r3, double green){

    glPushMatrix();
    glTranslated(x,y,z);
    glRotated(r, 1, 0, 0);
    glRotated(r2, 0, 1, 0);
    glRotated(r3, 0, 0, 1);
    glScaled(s,s,s);
    
    glColor3f(0, green, 0);
    sphere1(0, 0.5, 0, 0.7, 0.7, 0.7);
    
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    //float shiny[] = {1,1,1,1};
    //glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
    glBegin(GL_QUADS);
    /*glColor3f(0,green,0);
    glVertex3f(0, 1, 0);
    glVertex3f(0.5, 0, 0);
    glVertex3f(0, -0.7, 0);
    glVertex3f(-0.5, 0, 0);*/
    
    
    
    glColor3f(0.627, 0.322, 0.176);
    glNormal3d(0, 0, 1);
    glVertex3f(0.05, -0.2, 0.05);
    glVertex3f(-0.05, -0.2, 0.05);
    glVertex3f(-0.05, -1, 0.05);
    glVertex3f(0.05, -1, 0.05);
    
    glNormal3d(0, 0, -1);
    glVertex3f(0.05, -0.2, -0.05);
    glVertex3f(-0.05, -0.2, -0.05);
    glVertex3f(-0.05, -1, -0.05);
    glVertex3f(0.05, -1, -0.05);
    
    
   
    glNormal3d(1, 0, 0);
    glVertex3f(0.05, -0.2, 0.05);
    glVertex3f(0.05, -0.2, -0.05);
    glVertex3f(0.05, -1, -0.05);
    glVertex3f(0.05, -1, 0.05);
    
    glNormal3d(-1, 0, 0);
    glVertex3f(-0.05, -0.2, 0.05);
    glVertex3f(-0.05, -0.2, -0.05);
    glVertex3f(-0.05, -1, -0.05);
    glVertex3f(-0.05, -1, 0.05);
    
    glNormal3d(0, 1, 0);
    glVertex3f(-0.05, -0.2, 0.05);
    glVertex3f(-0.05, -0.2, -0.05);
    glVertex3f(0.05, -0.2, -0.05);
    glVertex3f(0.05, -0.2, 0.05);
    
    glNormal3d(0, -1, 0);
    glVertex3f(-0.05, -1, 0.05);
    glVertex3f(-0.05, -1, -0.05);
    glVertex3f(0.05, -1, -0.05);
    glVertex3f(0.05, -1, 0.05);
    glEnd();
    
    glPopMatrix();
    
}
static void branch(double x, double y, double z, double h, double r, double r2, double r3){
    
    glPushMatrix();
    glTranslated(x,y,z);
    glRotated(r, 1, 0, 0);
    glRotated(r2, 0, 1, 0);
    glRotated(r3, 0, 0, 1);
    glScaled(h,h,h);
    glBegin(GL_QUADS);
    //front
    glColor3f(0.627, 0.322, 0.176);
    glNormal3f(0, 0, 1);
    glVertex3f(-1.7, -0.5, 0);
    glVertex3f(-1.7, -1, 0);
    glVertex3f(0, -1, 0);
    glVertex3f(0, -0.5, 0);
    
    //front point
    //glColor3f(0.627, 0.322, 0.176);
    //line eq: 5x + 26z = 0
    glNormal3f(5, 0, 26);
    glVertex3f(0, -1, 0);
    glVertex3f(0, -0.5, 0);
    glVertex3f(1.3, -0.2, -0.25);
    glVertex3f(0, -1, 0);
    
    //back
    glNormal3f(0, 0, -1);
    glVertex3f(-1.7, -0.5, -0.5);
    glVertex3f(-1.7, -1, -0.5);
    glVertex3f(0, -1, -0.5);
    glVertex3f(0, -0.5, -0.5);
    
    //back point
    glNormal3f(5, 0, -26);
    //glColor3f(0.627, 0.322, 0.176);
    glVertex3f(0, -1, -0.5);
    glVertex3f(0, -0.5, -0.5);
    glVertex3f(1.3, -0.2, -0.25);
    glVertex3f(0, -1, -0.5);
    
    //end cap
    glNormal3f(-1, 0, 0);
    //glColor3f(0.545, 0.271, 0.075);
    glVertex3f(-1.7, -0.5, 0);
    glVertex3f(-1.7, -1, 0);
    glVertex3f(-1.7, -1, -0.5);
    glVertex3f(-1.7, -0.5, -0.5);
    
    //top
    glNormal3f(0, 1, 0);
    //glColor3f(0.804, 0.522, 0.247);
    glVertex3f(-1.7, -0.5, 0);
    glVertex3f(-1.7, -0.5, -0.5);
    glVertex3f(0, -0.5, -0.5);
    glVertex3f(0, -0.5, 0);
    
    //bottom
    glNormal3f(0, -1, 0);
    glColor3f(0.545, 0.271, 0.075);
    glVertex3f(-1.7, -1, 0);
    glVertex3f(-1.7, -1, -0.5);
    glVertex3f(0, -1, -0.5);
    glVertex3f(0, -1, 0);
    
    //top point
    //glColor3f(0.804, 0.522, 0.247);
    glNormal3f(-6, 26, 0);
    glVertex3f(0, -0.5, 0);
    glVertex3f(0, -0.5, -0.5);
    glVertex3f(1.3, -0.2, -0.25);
    glVertex3f(0, -0.5, 0);
    
    //bottom point
    //glColor3f(0.545, 0.271, 0.075);
    glNormal3f(8, -13, 0);
    glVertex3f(0, -1, 0);
    glVertex3f(0, -1, -0.5);
    glVertex3f(1.3, -0.2, -0.25);
    glVertex3f(0, -1, 0);
    
    glEnd();
    
    leaf(0.6, 0, 0, 0.6, 20, 40, 0, 0.6);
    leaf(0.6, 0, -0.8, 0.6, -50, -30, 0, 0.7);
    leaf(0.6, -0.7, 0.3, 0.6, 100,-20, 0, 0.4);
    leaf(-0.3, -1.2, 0.3, 0.6, 130,0,0, 0.5);
    leaf(-0.3, -0.8, 0.2, 0.4, 60,0,0, 0.9);
    leaf(-0.7, -0.2, -0.25, 0.6, 0,-70,0, 0.3);
    leaf(-0.9, -1, -0.6, 0.5, -100,0,0, 0.4);
    leaf(1.5, -0.23, -0.4, 0.4, 20,20,-90, 0.6);
    leaf(1.2, -0.04, -0.36, 0.3, 0,20,-30, 0.8);
    leaf(0.5, -1.2, 0, 0.62, 0, -30, -150, 0.3);
    leaf(0.5, -1, -0.6, 0.4, 0, 30, -110, 0.8);
    leaf(-0.5, -1.2, -0.3, 0.42, 0, 0, -180, 0.8);
    leaf(-1.2, -0.2, -0.3, 0.52, 0, -30, 30, 0.6);
    leaf(-0.2, -1.2, -0.7, 0.62, 30, -30, -190, 0.9);
    leaf(-0.2, -0.4, -0.7, 0.62, 0, 40, -45, 0.5);
    glPopMatrix();
}


static void apple(double x, double y, double z, double h, double r){
    //used to make the apple ring
    const int d=15;

    //  Save transformation
    glPushMatrix();
    //  Offset and scale
    glTranslated(x,y,z);
    glRotated(h, 1, 0, 0);
    glScaled(r,r,r);
    
    glBegin(GL_QUADS);
    glColor3f(0,1,0);
    glNormal3f(0, 0, 1);
    glVertex3f(-0.1,0, 0);
    glVertex3f(+0.1,0, 0);
    glVertex3f(+0.1,1.5, 0);
    glVertex3f(-0.1,1.5, 0);
    
    glNormal3f(0, 0, 1);
    glVertex3f(0, 0.9, 0);
    glVertex3f(0.5, 1.0, 0);
    glVertex3f(0.6, 1.4, 0);
    glVertex3f(0.3, 1.3, 0);
    
    
    glEnd();
    //  Latitude bands
    glColor3f(1,0,0);
    for (int ph=-90;ph<90;ph+=d)
    {
       glBegin(GL_QUAD_STRIP);
       for (int th=0;th<=360;th+=d)
       {
          Vertex(th,ph);
          Vertex(th,ph+d);
       }
       glEnd();
    }
    
    //  Undo transformations
    glPopMatrix();
    
    
}

static void tree(double x, double y, double z, double s, double r){
    glPushMatrix();
    //  Offset and scale
    glTranslated(x,y,z);
    glRotated(r, 0, 1, 0);
    glScaled(s,s,s);
    glBegin(GL_QUADS);
    glColor3f(0.6, 0.3, 0.1);
    
    //front
    glNormal3f(0, 2, 12);
    glVertex3f(-0.1, 0, 0.25);
    glVertex3f(0.1, 0, 0.25);
    glVertex3f(0.35, -1.5, 0.5);
    glVertex3f(-0.35, -1.5, 0.5);
    
    //back
    
    //glColor3f(0.7, 0.4, 0.2);
    glNormal3f(0, 2, -12);
    glVertex3f(-0.1, 0, -0.25);
    glVertex3f(0.1, 0, -0.25);
    glVertex3f(0.35, -1.5, -0.5);
    glVertex3f(-0.35, -1.5, -0.5);
    
    //left
    //glColor3f(0.627, 0.322, 0.176);
    glNormal3f(-12, 2, 0);
    glVertex3f(-0.25, 0, -0.1);
    glVertex3f(-0.25, 0, 0.1);
    glVertex3f(-0.5, -1.5, 0.35);
    glVertex3f(-0.5, -1.5, -0.35);
    
    //right
    //glColor3f(0.627, 0.4, 0.176);
    glNormal3f(12, 2, 0);
    glVertex3f(0.25, 0, -0.1);
    glVertex3f(0.25, 0, 0.1);
    glVertex3f(0.5, -1.5, 0.35);
    glVertex3f(0.5, -1.5, -0.35);
    
    //corner fl
    //glColor3f(0.545, 0.271, 0.075);
    glNormal3f(-60, 20, 60);
    glVertex3f(-0.1, 0, 0.25);
    glVertex3f(-0.25, 0, 0.1);
    glVertex3f(-0.5, -1.5, 0.35);
    glVertex3f(-0.35, -1.5, 0.5);
    
    
    //glColor3f(0.545, 0.271, 0.075);
    glNormal3f(60, 20, 60);
    glVertex3f(0.1, 0, 0.25);
    glVertex3f(0.25, 0, 0.1);
    glVertex3f(0.5, -1.5, 0.35);
    glVertex3f(0.35, -1.5, 0.5);
    
    
    //glColor3f(0.627, 0.322, 0.176);
    glNormal3f(-60, 20, -60);
    glVertex3f(-0.1, 0, -0.25);
    glVertex3f(-0.25, 0, -0.1);
    glVertex3f(-0.5, -1.5, -0.35);
    glVertex3f(-0.35, -1.5, -0.5);
    
    
    //glColor3f(0.545, 0.271, 0.075);
    glNormal3f(60, 20, -60);
    glVertex3f(0.1, 0, -0.25);
    glVertex3f(0.25, 0, -0.1);
    glVertex3f(0.5, -1.5, -0.35);
    glVertex3f(0.35, -1.5, -0.5);
    
    glNormal3f(0, 2, -12);
    glVertex3f(-0.1, 0, 0.25);
    glVertex3f(0.1, 0, 0.25);
    glVertex3f(0.35, -1.5, 0.5);
    glVertex3f(-0.35, -1.5, 0.5);
    
    //begin flipped
    glColor3f(0.6, 0.3, 0.1);
    
    //front
    glNormal3f(0, -2, 12);
    glVertex3f(-0.1, 0, 0.25);
    glVertex3f(0.1, 0, 0.25);
    glVertex3f(0.35, 1.5, 0.5);
    glVertex3f(-0.35, 1.5, 0.5);
    //back
    //glColor3f(0.7, 0.4, 0.2);
    glNormal3f(0, -2, -12);
    glVertex3f(-0.1, 0, -0.25);
    glVertex3f(0.1, 0, -0.25);
    glVertex3f(0.35, 1.5, -0.5);
    glVertex3f(-0.35, 1.5, -0.5);
    
    //left
    //glColor3f(0.627, 0.322, 0.176);
    glNormal3f(-12, -2, 0);
    glVertex3f(-0.25, 0, -0.1);
    glVertex3f(-0.25, 0, 0.1);
    glVertex3f(-0.5, 1.5, 0.35);
    glVertex3f(-0.5, 1.5, -0.35);
    
    
    //right
    //glColor3f(0.627, 0.4, 0.176);
    glNormal3f(12, -2, 0);
    glVertex3f(0.25, 0, -0.1);
    glVertex3f(0.25, 0, 0.1);
    glVertex3f(0.5, 1.5, 0.35);
    glVertex3f(0.5, 1.5, -0.35);
    
    //corner fl
   // glColor3f(0.545, 0.271, 0.075);
    glNormal3f(-60, -20, 60);
    glVertex3f(-0.1, 0, 0.25);
    glVertex3f(-0.25, 0, 0.1);
    glVertex3f(-0.5, 1.5, 0.35);
    glVertex3f(-0.35, 1.5, 0.5);
    
    
    //glColor3f(0.545, 0.271, 0.075);
    glNormal3f(60, -20, 60);
    glVertex3f(0.1, 0, 0.25);
    glVertex3f(0.25, 0, 0.1);
    glVertex3f(0.5, 1.5, 0.35);
    glVertex3f(0.35, 1.5, 0.5);
    
    
    //glColor3f(0.627, 0.322, 0.176);
    glNormal3f(-60, -20, -60);
    glVertex3f(-0.1, 0, -0.25);
    glVertex3f(-0.25, 0, -0.1);
    glVertex3f(-0.5, 1.5, -0.35);
    glVertex3f(-0.35, 1.5, -0.5);
    
    
    //glColor3f(0.545, 0.271, 0.075);
    glNormal3f(60, 20, -60);
    glVertex3f(0.1, 0, -0.25);
    glVertex3f(0.25, 0, -0.1);
    glVertex3f(0.5, 1.5, -0.35);
    glVertex3f(0.35, 1.5, -0.5);
    
    glNormal3f(0, -2, -12);
    glVertex3f(-0.1, 0, 0.25);
    glVertex3f(0.1, 0, 0.25);
    glVertex3f(0.35, 1.5, 0.5);
    glVertex3f(-0.35, 1.5, 0.5);
    //end flipped
    
    //START CHANGING HERE
    //begin top cap
    //glColor3f(0.627, 0.322, 0.176);
    glNormal3f(0, 1, 0);
    glVertex3f(-0.5, 1.5, 0.35);
    glVertex3f(-0.35, 1.5, 0.5);
    glVertex3f(0.35, 1.5, 0.5);
    glVertex3f(0.5, 1.5, 0.35);
    
    //glColor3f(0.627, 0.322, 0.176);
    glNormal3f(0, 1, 0);
    glVertex3f(-0.5, 1.5, -0.35);
    glVertex3f(-0.35, 1.5, -0.5);
    glVertex3f(0.35, 1.5, -0.5);
    glVertex3f(0.5, 1.5, -0.35);
    
    //glColor3f(0.627, 0.322, 0.176);
    glNormal3f(0, 1, 0);
    glVertex3f(0.5, 1.5, -0.35);
    glVertex3f(0.5, 1.5, 0.35);
    glVertex3f(-0.5, 1.5, 0.35);
    glVertex3f(-0.5, 1.5, -0.35);
    
    //end top cap
    
    //begin bottom cap
    //glColor3f(0.627, 0.322, 0.176);
    glNormal3f(0, -1, 0);
    glVertex3f(-0.5, -1.5, 0.35);
    glVertex3f(-0.35, -1.5, 0.5);
    glVertex3f(0.35, -1.5, 0.5);
    glVertex3f(0.5, -1.5, 0.35);
    
    glNormal3f(0, -1, 0);
    //glColor3f(0.627, 0.322, 0.176);
    glVertex3f(-0.5, -1.5, -0.35);
    glVertex3f(-0.35, -1.5, -0.5);
    glVertex3f(0.35, -1.5, -0.5);
    glVertex3f(0.5, -1.5, -0.35);
    
    glNormal3f(0, -1, 0);
    //glColor3f(0.627, 0.322, 0.176);
    glVertex3f(0.5, -1.5, -0.35);
    glVertex3f(0.5, -1.5, 0.35);
    glVertex3f(-0.5, -1.5, 0.35);
    glVertex3f(-0.5, -1.5, -0.35);
    //end bottom cap
    glEnd();
    
    //big 4
    branch(1.2, 1.74, 0, 0.5, 0,0,0);
    branch(-1.2, 1.74, 0, 0.5, 0,180,0);
    branch(0, 1.74, 1.2, 0.5, 0,-90,0);
    branch(0, 1.74, -1.2, 0.5, 0,90,0);
    
    //big 4 out of top
    branch(0.5, 2.2, -0.23, 0.6, 0,45,45);
    branch(0.3, 2.2, 0.5, 0.6, 0,-45,45);
    branch(-0.5, 2.2, 0.23, 0.6, 0,225,45);
    branch(-0.3, 2.2, -0.5, 0.6, 0,-225,45);
    
    //smaller
    branch(1, 1.8, -0.3, 0.3, 0, 45, 45);
    
    branch(0.9, 1.3, 0.6, 0.4, 0, -20, -20);
    branch(-0.9, 1.3, -0.6, 0.4, 0, -200, -20);
    branch(-0.3, 0.8, 0.6, 0.3, 0, -110, 5);
    branch(0.4, 0.8, -0.6, 0.35, 0, 70, 10);
    
  // small on the big 4 out of top
    branch(0.1, 1.8, 0.5, 0.3, 0, -132, 20);
    branch(0.3, 2.4, 0.5, 0.3, 0, -30, 90);
    branch(-0.7, 1.2, 0.7, 0.4, 0, 45, 180);
    
    apple(-0.2,0.25,0.6, 0, 0.15);
    apple(-0.2,1.2,-1.4, 0, 0.15);
    apple(0.5,0.25,-0.6, 0, 0.15);
    glPopMatrix();
    
    
    
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void reset(){
    if(mode==0){
        dim = 13;
        ph = 30;
        th = 45;
    }
    else if(mode==2){
        dim = 5;
        th = 180;
        ph = 0;
        cameraX = 0;
        cameraZ = -7;
        
    }
    else if(mode==1){
        dim = 13;
        ph = 30;
        th = 45;
        
    }
    
    
}
static void redPanda(double x, double y, double z, double s){
    glPushMatrix();
    //  Offset and scale
    glTranslated(x,y,z);
    glRotated(0, 0, 0, 1);
    glScaled(s,s,s);
    glColor3f(1, 0.55, 0);
    sphere1(0, 1, 0, 1.3, 1, 1);//head
    sphere1(0, 0, 0, 0.5, 0.7, 0.5);//body
    glColor3f(0.3, 0.2, 0.1);
    sphere1(0.4, -0.65, 0, 0.25, 0.15, 0.25);//feet
    sphere1(-0.4, -0.65, 0, 0.25, 0.15, 0.25);//feet
    sphere1(0, 0.51, 1.14, 0.08, 0.12, 0.03);
    glColor3f(1, 0.7, 0);
    //glPushMatrix();
    //glRotated(10,0, 0,0);
    //sphere1(0, -0.3, -0.8, 0.25, 0.25, 0.7);//tail, turned into tail function
    //glPopMatrix();
    
    glColor3f(1, 1, 1);
    sphere1(0, 0.6, 0.75, 0.5,0.5, 0.4);//snout
    
    tail(0,-0.3,-0.8,0.25,0.25,0.7);
    ears(1.2,1.55,0,1,1,0.4);
    ears(-1.2, 1.55, 0, -1,1, 0.4);
    ears(1.2,1.1,0.2,1,-1,0.2);
    ears(-1.2, 1.1, 0.2, -1,-1, 0.2);
    
    glColor3f(0.3, 0.2,0.1);
    sphere1(0, 0.2, 0.42, 0.3, 0.7, 0.1);
    glPushMatrix();
    glRotated(45,-1,0.5,-0.3);
    sphere1(0.5,0,0, 0.2,0.4,0.2);
    glPopMatrix();
    glPushMatrix();
    glRotated(45,-1,-0.5,0.3);
    sphere1(-0.5,0,0, 0.2,0.4,0.2);
    glPopMatrix();
    glBegin(GL_QUADS);
    
    glColor3f(1,1,1);
    glNormal3f(0,0,1);
    glVertex3f(0.53,0.75, 0.9);//top left
    glVertex3f(0.6,0.85,0.9);//top right
    glVertex3f(0.8,0.5, 0.7);//bottom right
    glVertex3f(0.6,0.5, 0.74);//bottom left
    
    //**this purposely clips into the face**
    glVertex3f(0.6, 0.5, 0.74);
    glVertex3f(0.8, 0.5, 0.7);
    glVertex3f(0.53, 0.3, 0.6);
    glVertex3f(0.6, 0.3, 0.6);
    
    glVertex3f(-0.53,0.75, 0.9);//top left
    glVertex3f(-0.6,0.85,0.9);//top right
    glVertex3f(-0.8,0.5, 0.7);//bottom right
    glVertex3f(-0.6,0.5, 0.74);//bottom left
    
    //**this purposely clips into the face**
    glVertex3f(-0.6, 0.5, 0.74);
    glVertex3f(-0.8, 0.5, 0.7);
    glVertex3f(-0.53, 0.3, 0.6);
    glVertex3f(-0.6, 0.3, 0.6);
    
    //eyebrows
    glVertex3f(0.35, 1.15, 0.95); //left bottom
    glVertex3f(0.45, 1.4, 0.86); //middle top
    glVertex3f(0.7, 1.15, 0.84); //right bottom
    glVertex3f(0.51, 1.25, 0.9); //middle bottom
    
    glVertex3f(-0.35, 1.15, 0.95);
    glVertex3f(-0.45, 1.4, 0.86);
    glVertex3f(-0.7, 1.15, 0.84);
    glVertex3f(-0.51, 1.25, 0.9);
    
    glColor3f(1,1,1);
    glVertex3f(1.1, 1.8, 0.44);
    glVertex3f(1.35, 1.65, 0.44);
    glVertex3f(1.42, 1.4, 0.44);
    glColor3f(0.5,0.5,0.5);
    glVertex3f(1.55, 1.8, 0.44);
    
    glColor3f(1,1,1);
    glVertex3f(-1.1, 1.8, 0.44);
    glVertex3f(-1.35, 1.65, 0.44);
    glVertex3f(-1.42, 1.4, 0.44);
    glColor3f(0.5,0.5,0.5);
    glVertex3f(-1.55, 1.8, 0.44);
    
    //
  
    
    glEnd();
    
    glColor3f(0.3, 0.2, 0.1);
    sphere1(0.5, 0.9, 0.95, 0.1, 0.2, 0.1);//eye1
    sphere1(-0.5, 0.9, 0.95, 0.1, 0.2, 0.1);//eye2
    sphere1(0, 0.8, 1.15, 0.15, 0.1, 0.1);//nose
    
    glPopMatrix();
    
}
static void floorDraw(){
    glBegin(GL_QUADS);
    glColor3f(0,0.4,0);
    
    
    //top
    /*glVertex3f(10, -2, 10);
    glVertex3f(-10, -2, 10);
    glVertex3f(-10, -2, -10);
    glVertex3f(10, -2, -10);*/
    //glColor3f(1,1,1);
    for(int i = -10; i<10; i++){
        for(int j = 10; j<10; j++){
            
            glVertex3f(i, -2, j);
            glVertex3f(i+1, -2, j);
            glVertex3f(i+1, -2, j+1);
            glVertex3f(i, -2, j+1);
            
        }
        
    }
    glNormal3f(0,1,0);
    glVertex3f(-10, -2, -10);
    glVertex3f(-10, -2, -5);
    glVertex3f(-5, -2, -5);
    glVertex3f(-5, -2, -10);
    
    glVertex3f(-10, -2, -5);
    glVertex3f(-10, -2, -0);
    glVertex3f(-5, -2, -0);
    glVertex3f(-5, -2, -5);
    
    glVertex3f(-10, -2, 0);
    glVertex3f(-10, -2, 5);
    glVertex3f(-5, -2, 5);
    glVertex3f(-5, -2, 0);
    
    glVertex3f(-10, -2, 10);
    glVertex3f(-10, -2, 5);
    glVertex3f(-5, -2, 5);
    glVertex3f(-5, -2, 10);
    
    //
    glVertex3f(-5, -2, -5);
    glVertex3f(-5, -2, 0);
    glVertex3f(0, -2, 0);
    glVertex3f(0, -2, -5);
    
    glVertex3f(-5, -2, -10);
    glVertex3f(-5, -2, -5);
    glVertex3f(0, -2, -5);
    glVertex3f(0, -2, -10);
    
    glVertex3f(-5, -2, 5);
    glVertex3f(-5, -2, 0);
    glVertex3f(0, -2, 0);
    glVertex3f(0, -2, 5);
    
    glVertex3f(-5, -2, 5);
    glVertex3f(-5, -2, 10);
    glVertex3f(0, -2, 10);
    glVertex3f(0, -2, 5);
    //
    
    glVertex3f(0, -2, 0);
    glVertex3f(0, -2, 5);
    glVertex3f(5, -2, 5);
    glVertex3f(5, -2, 0);
    
    glVertex3f(0, -2, -10);
    glVertex3f(0, -2, -5);
    glVertex3f(5, -2, -5);
    glVertex3f(5, -2, -10);
    
    glVertex3f(0, -2, 0);
    glVertex3f(0, -2, -5);
    glVertex3f(5, -2, -5);
    glVertex3f(5, -2, 0);
    
    glVertex3f(0, -2, 10);
    glVertex3f(0, -2, 5);
    glVertex3f(5, -2, 5);
    glVertex3f(5, -2, 10);
    
    //
    
    glVertex3f(10, -2, 10);
    glVertex3f(10, -2, 5);
    glVertex3f(5, -2, 5);
    glVertex3f(5, -2, 10);
    
    glVertex3f(10, -2, -10);
    glVertex3f(10, -2, -5);
    glVertex3f(5, -2, -5);
    glVertex3f(5, -2, -10);
    
    glVertex3f(10, -2, 0);
    glVertex3f(10, -2, -5);
    glVertex3f(5, -2, -5);
    glVertex3f(5, -2, 0);
    
    glVertex3f(10, -2, 0);
    glVertex3f(10, -2, 5);
    glVertex3f(5, -2, 5);
    glVertex3f(5, -2, 0);
    
    
    glColor3f(0,0.4,0);
    //bottom
    glNormal3f(0, -1, 0);
    glVertex3f(10, -2.4, 10);
    glVertex3f(-10, -2.4, 10);
    glVertex3f(-10, -2.4, -10);
    glVertex3f(10, -2.4, -10);
    
    //front
    glNormal3f(0,0,1);
    glColor3f(0, 0.2, 0);
    glVertex3f(10, -2, 10);
    glVertex3f(-10, -2, 10);
    glVertex3f(-10, -2.4, 10);
    glVertex3f(10, -2.4, 10);
    
    //back
    glNormal3f(0,0,-1);
    glVertex3f(10, -2, -10);
    glVertex3f(-10, -2, -10);
    glVertex3f(-10, -2.4, -10);
    glVertex3f(10, -2.4, -10);
    
    //left
    glNormal3f(-1, 0, 0);
    glVertex3f(10, -2, 10);
    glVertex3f(10, -2, -10);
    glVertex3f(10, -2.4, -10);
    glVertex3f(10, -2.4, 10);
    
    //right
    glNormal3f(1, 0, 0);
    glVertex3f(-10, -2, 10);
    glVertex3f(-10, -2, -10);
    glVertex3f(-10, -2.4, -10);
    glVertex3f(-10, -2.4, 10);
    
    
    glEnd();
    
    
}
/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
    //glClearColor(0.6,1,0.7,0);
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
    //glClearColor(0.6,1,0.7,0);
   //  Perspective - set eye position
    
   if (mode==2)
   {
       double Ex = -2*dim*Sin(th)*Cos(ph);
       double Ey = +2*dim        *Sin(ph);
       double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(-cameraX,0,-cameraZ , Ex-cameraX,Ey ,Ez-cameraZ, 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else if(mode == 0)
   {
      
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }
   else if(mode ==1){
       
          double Ex = -2*dim*Sin(th)*Cos(ph);
          double Ey = +2*dim        *Sin(ph);
          double Ez = +2*dim*Cos(th)*Cos(ph);
          gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
       
   }
    
    glShadeModel(GL_SMOOTH);
    
    float Ambient[] = {0.01*ambient, 0.01*ambient, 0.01*ambient, 1.0};
    float Diffuse[] = {0.01*r, 0.01*g, 0.01*b, 1.0};
    float Specular[] = {0.001*spec, 0.001*spec, 0.001*spec, 1.0};
    int local = 0;
    
    float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
    
    //  Draw light position as ball (still no lighting here)
    glColor3f(r/50,g/50,b/50);
    sphere1(Position[0],Position[1],Position[2] , 0.1, 0.1, 0.1);
    
    
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
    
    
    //branch(0, 0, 0, 1, 0, 0, 0);
    //tree(0, 0, 0, 1, 0);
    //redPanda(0,0,0,1);
    if(obj == 0){
    tree(-9,-0.5,5,1,-45);
    tree(-7,-0.5,-5,1,90);
    tree(-5,-0.5,3,1,111);
    tree(-3, -0.9, -2, 0.7, 242);
    tree(-8, -0.8, -1, 0.8, 212);
    tree(1, -0.5, 4, 1, 9);
    tree(-1.5, -0.5, 7, 1, 0);
    tree(4, -0.5, -6, 1, 15);
    tree(6, -0.9, 6, 0.7, 43);
    tree(-1, -0.5, -7, 1, 5);
    tree(4, -0.5, -1, 1, 95);
    tree(7, -0.5, 3, 1, 0);
    
    redPanda(0,-1.2, 0, 0.4);
    
    //apple pile
//bottom circle
    apple(0, -1.85, 0.45, 30, 0.15);
    apple(0.3, -1.85, 0.4, 90, 0.15);
    apple(0.5, -1.85, 0.2, -50, 0.15);
    apple(0.68, -1.85, 0, 70, 0.15);
    
   
    apple(-0.3, -1.85, 0.4, -70, 0.15);
    apple(-0.5, -1.85, 0.2, 60, 0.15);
    apple(-0.68, -1.85, 0, -80, 0.15);
    
    apple(0, -1.85, -0.45, 150, 0.15);
    apple(0.3, -1.85, -0.4, -30, 0.15);
    apple(0.5, -1.85, -0.2, 130, 0.15);
   
    apple(-0.3, -1.85, -0.4, 50, 0.15);
    apple(-0.5, -1.85, -0.2, 20, 0.15);
    
    //upper circle
    apple(0.1, -1.65, 0.3, -60, 0.15);
    apple(0.35, -1.65, 0.15, 60, 0.15);
    apple(0.4, -1.65, -0.1, 70, 0.15);
    apple(0.2, -1.65, -0.3, 30, 0.15);
    apple(-0.1, -1.65, 0.3, 20, 0.15);
    apple(-0.35, -1.65, 0.15, 90, 0.15);
    apple(-0.4, -1.65, -0.1, 30, 0.15);
    apple(-0.2, -1.65, -0.3, 50, 0.15);
    
    //center apples
    apple(0, -1.65, 0, 90, 0.15);
    
    //scattered apples
    apple(0.4, -1.85, 1, 45, 0.15);
    apple(1, -1.85, -1, 130, 0.15);
    apple(-0.2, -1.85, -0.8, 90, 0.15);
    apple(0.4, -1.85, -0.7, -95, 0.15);
    apple(-0.8, -1.85, 0.3, -45, 0.15);
    
    //scattered away from pile
    apple(4, -1.85, 7, -45, 0.15);
    apple(-4, -1.85, 3, -45, 0.15);
    apple(8, -1.85, 2, -45, 0.15);
    apple(5, -1.85, -5, -45, 0.15);
    apple(-7, -1.85, -7, -45, 0.15);
        floorDraw();}
    
    else if(obj == 1){
        tree(0,0,0,1,0);
        
    }
    else if(obj == 2){
        redPanda(0,0,0,1);
        
    }
    else if(obj==3){
        apple(0,0,0,0,1);
        
    }
    
    
    //walls for testing
 
    glColor3f(r/50,g/50,b/50);

    glDisable(GL_LIGHTING);
   //  Display parameters
   glWindowPos2i(5,5);
   //Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
    
    if(rainbow==0){
        
        Print("Ambient: %.1f, Specular: %.1f, Light Dist/Height: %.1f,%.1f Color: White", ambient, spec, distance, ylight);
        
    }
    else if(rainbow==1){
      
        Print("Ambient: %.1f, Specular: %.1f, Light Dist/Height: %.1f,%.1f Color: Red", ambient, spec, distance, ylight);
        
    }
    else if(rainbow==2){
        
        Print("Ambient: %.1f, Specular: %.1f, Light Dist/Height: %.1f,%.1f Color: Orange", ambient, spec, distance, ylight);
        
    }
    else if(rainbow==3){
       
        Print("Ambient: %.1f, Specular: %.1f, Light Dist/Height: %.1f,%.1f Color: Yellow", ambient, spec, distance, ylight);
        
    }
    else if(rainbow==4){
       
        Print("Ambient: %.1f, Specular: %.1f, Light Dist/Height: %.1f,%.1f Color: Green", ambient, spec, distance, ylight);
        
    }
    else if(rainbow==5){
        
        Print("Ambient: %.1f, Specular: %.1f, Light Dist/Height: %.1f,%.1f Color: Cyan", ambient, spec, distance, ylight);
        
    }
    else if(rainbow==6){
        
        Print("Ambient: %.1f, Specular: %.1f, Light Dist/Height: %.1f,%.1f Color: Blue", ambient, spec, distance, ylight);
        
    }
    else if(rainbow==7){
        
        Print("Ambient: %.1f, Specular: %.1f, Light Dist/Height: %.1f,%.1f Color: Purple", ambient, spec, distance, ylight);
        
    }
  
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void lightReset(){
    rainbow = 0;
    ambient = 10;
    spec = 10;
    r = 50;
    g = 50;
    b = 50;
    distance = 4;
    ylight = 0;
    
}
void idle()
{
    if(move){
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
        glutPostRedisplay();
        
    }
}

void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT){
        th += 5;
        
    }
      
   //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT){
        th -= 5;
        
    }
   //  Up arrow key - move forwards
    else if (key == GLUT_KEY_UP){
        
        
            ph+=5;
        
    }
    
   //  Down arrow key - move backwards
   else if (key == GLUT_KEY_DOWN){
      
           ph-=5;
       
    }
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
    if(mode==2){
        if(ph>=75) ph = 75;
        else if (ph<=-60) ph = -60;
        
    }
   ph %= 360;
   //  Update projection
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
   else if (ch == '0')
       reset();
   //  Toggle axes
   //else if (ch == 'a' || ch == 'A')
      //axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M'){
       mode++;
    if (mode==3) {
        mode = 0;
        
    }
       reset();
   }
   else if (ch == 'w'||ch == 'W'){
       if(mode ==2){
           cameraX += Sin(th);
           cameraZ -=Cos(th);
           
       }
       else if(dim>1) dim-=0.6;
       
       
   }
   else if (ch == 's'||ch == 'S'){
       if(mode ==2){
           cameraX += Sin(th+180);
           cameraZ -=Cos(th+180);
           
       }
       else dim+=0.6;
       
   }
   else if (ch == 'a'||ch == 'A'){
       if(mode ==2){
           cameraX -= Sin(th+90);
           cameraZ +=Cos(th+90);
           
       }
       
   }
   else if (ch == 'd'||ch == 'D'){
       if(mode ==2){
           cameraX += Sin(th+90);
           cameraZ -=Cos(th+90);
           
       }
    
       
   }
   else if(ch == '[') ylight++;
   else if(ch == ']') ylight--;
   else if(ch=='1') distance++;
   else if(ch == '2') distance--;
   else if (ch=='3') ambient+=5;
   else if(ch=='4'&&ambient>4) ambient-=5;
   else if(ch=='5') spec +=10;
   else if(ch =='6') spec-=10;
   else if(ch=='r'|ch=='R'){
       if(rainbow<7) rainbow++;
       else rainbow = 0;
       if(rainbow==0){
           //white
           r=50;
           g=50;
           b=50;
       }
       else if(rainbow==1){
           //red
           r = 50;
           g = 0;
           b = 0;
           
       }
       else if(rainbow==2){
           //orange
           r = 50;
           g = 20;
           b = 0;
       }
       else if(rainbow==3){
           //yellow
           r = 50;
           g = 50;
           b = 0;
       }
       else if(rainbow==4){
           //green
           r = 0;
           g = 50;
           b = 0;
           
       }
       else if(rainbow==5){
           //cyan
           r = 0;
           g = 50;
           b = 50;
           
       }
       else if(rainbow==6){
           //blue
           r = 0;
           g = 10;
           b = 70;
           
       }
       else if(rainbow==7){
           //purple
           r = 20;
           g = 0;
           b = 50;
           
       }
     
   }
    
   
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   else if(ch == 'o'||ch == 'O'){
       obj++;
       if(obj>3) obj = 0;
   }
   else if(ch == '9') lightReset();
   else if(ch=='8') move = !move;
   //  Reproject
   Project();
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
   glViewport(0,0, RES*width,RES*height);
   //  Set projection
   Project();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Kate McCarthy HW5");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
    
    glutIdleFunc(move?idle:NULL);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
