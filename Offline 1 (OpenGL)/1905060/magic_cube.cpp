#ifdef __linux__
   #include <GL/glut.h>
#elif WIN32
   #include <windows.h>
   #include <glut.h>
#endif

#include <cmath>
#include <iostream>

using namespace std;

#define pi (2 * acos(0.0))
#define rotation_angle pi / 180.0

double maxTriangleLength = 1.6;

double maxSphereRadius = maxTriangleLength / sqrt(3.0);

double triangleLength = 1.6, sphere_radius = 0;

double sphereStep = maxSphereRadius / 16.0;

int numSegments = 36;

class Point
{
public:
   Point()
   {
      this->x = 0;
      this->y = 0;
      this->z = 0;
   }

   double x, y, z;

   Point(double x, double y, double z)
   {
      this->x = x;
      this->y = y;
      this->z = z;
   }

   Point(const Point &p)
   {
      this->x = p.x;
      this->y = p.y;
      this->z = p.z;
   }

   // arithemtic operations
   Point operator+(Point b)
   {
      Point p;
      p.x = x + b.x;
      p.y = y + b.y;
      p.z = z + b.z;
      return p;
   }
   Point operator-(Point b)
   {
      Point p;
      p.x = x - b.x;
      p.y = y - b.y;
      p.z = z - b.z;
      return p;
   }
   Point operator*(double b)
   {
      Point p;
      p.x = x * b;
      p.y = y * b;
      p.z = z * b;
      return p;
   }
   Point operator/(double b)
   {
      Point p;
      p.x = x / b;
      p.y = y / b;
      p.z = z / b;
      return p;
   }
   Point operator*(Point b)
   {
      Point p;
      p.x = y * b.z - z * b.y;
      p.y = z * b.x - x * b.z;
      p.z = x * b.y - y * b.x;
      return p;
   }

   void set_value(double x, double y, double z)
   {
      this->x = x;
      this->y = y;
      this->z = z;
   }
};

Point normalize(Point a)
{
   double dot_product = a.x * a.x + a.y * a.y + a.z * a.z;
   double length = sqrt(dot_product);
   return a / length;
}

// Global variables
Point eye_position, look, r, up; // position of the eye

double angleRotationZ = 0.0;
/* Initialize OpenGL Graphics */
void initGL()
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
   glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling

   eye_position.set_value(5, 0, 2);
   look.set_value(-1, 0, 0);
   r.set_value(0, 1, 0);
   up.set_value(0, 0, 1);
}

void drawTriangle()
{

   glBegin(GL_TRIANGLES);
   {
      glVertex3f(1, 0, 0);
      glVertex3f(0, 1, 0);
      glVertex3f(0, 0, 1);
   }
   glEnd();
}

void draw_cylinder_helper(int segments, double radius, double height)
{
   struct Point points[segments + 1];

   double offset = 70.5287794 * M_PI / 180.0;

   int i = 0;
   while (i < segments + 1)
   {
      points[i].x = radius * cos(-offset / 2 + i * offset / segments);
      points[i].y = radius * sin(-offset / 2 + i * offset / segments);
      i++;
   }

   height = height / 2;

   glBegin(GL_QUADS);
   {
      int i = 0;
      while (i < segments)
      {
         glVertex3f(points[i].x, points[i].y, height);
         glVertex3f(points[i].x, points[i].y, -height);
         glVertex3f(points[i + 1].x, points[i + 1].y, -height);
         glVertex3f(points[i + 1].x, points[i + 1].y, height);
         i++;
      }
   }
   glEnd();
}

void drawSphereQuad(double radius, int segments)
{

   struct Point points[segments + 1][segments + 1];
   int i = 0, j = 0;
   double x, y;

   while (i <= segments)
   {
      j = 0;
      while (j <= segments)
      {
         points[i][j].x = -1 + (double)i / segments * 2;
         points[i][j].y = -1 + (double)j / segments * 2;
         points[i][j].z = 1;

         points[i][j] = normalize(points[i][j]);
         points[i][j] = points[i][j] * radius;
         j++;
      }
      i++;
   }

   i = 0;

   while (i < segments)
   {
      j = 0;
      while (j < segments)
      {
         glBegin(GL_QUADS);
         {
            glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
            glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
            glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
            glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
         }
         glEnd();
         j++;
      }
      i++;
   }
}

void drawCylinders()
{

   glColor3f(1.0f, 1.0f, 0.0f);

   double x = triangleLength / sqrt(2);

   double y = triangleLength * sqrt(2);

   int i = 0;

   while (i < 4)
   {
      glPushMatrix();
      {
         glRotatef(45 + i * 90, 0, 1, 0);
         glTranslatef(x, 0, 0);
         draw_cylinder_helper(100, sphere_radius, y);
      }
      glPopMatrix();
      i++;
   }

   i = 0;

   while (i < 4)
   {
      glPushMatrix();
      {
         glRotatef(90, 1, 0, 0);
         glRotatef(45 + i * 90, 0, 1, 0);
         glTranslatef(x, 0, 0);
         draw_cylinder_helper(100, sphere_radius, y);
      }
      glPopMatrix();
      i++;
   }

   i = 0;

   while (i < 4)
   {
      glPushMatrix();
      {
         glRotatef(90, 0, 0, 1);
         glRotatef(45 + i * 90, 0, 1, 0);
         glTranslatef(x, 0, 0);
         draw_cylinder_helper(100, sphere_radius, y);
      }
      glPopMatrix();
      i++;
   }
}

void drawSpheres()
{

   int i = 0;

   while (i < 4)
   {
      glPushMatrix();
      {
         glColor3f(0, i % 2, (i + 1) % 2); // blue / green
         glRotatef(90 * i, 0, 1, 0);
         glTranslatef(0, 0, triangleLength);
         drawSphereQuad(sphere_radius, 100);
      }
      glPopMatrix();
      i++;
   }

   i = 0;

   while (i < 2)
   {
      glPushMatrix();
      {
         glColor3f(1.0f, 0.0f, 0.0f); // red
         glRotatef(90 + (180 * i), 1, 0, 0);
         glTranslatef(0, 0, triangleLength);
         drawSphereQuad(sphere_radius, 100);
      }
      glPopMatrix();
      i++;
   }
}

void drawPyramids()
{
   double difference = (maxTriangleLength - triangleLength) / 3.0;

   int i = 0;

   while (i < 4)
   {
      glPushMatrix();
      {
         glColor3f((i + 1) % 2, i % 2, 1.0f); // purple / cyan
         glRotatef(90 * i, 0, 1, 0);
         glTranslatef(difference, difference, difference);
         glScaled(triangleLength, triangleLength, triangleLength);
         drawTriangle();
      }
      glPopMatrix();

      i++;
   }

   i = 0;

   while (i < 4)
   {
      glPushMatrix();
      {
         glColor3f(i % 2, (i + 1) % 2, 1.0f); // cyan / pruple
         glRotatef(90 * i, 0, 1, 0);
         glRotatef(180, 1, 0, 1);
         glTranslatef(difference, difference, difference);
         glScaled(triangleLength, triangleLength, triangleLength);
         drawTriangle();
      }
      glPopMatrix();

      i++;
   }
}


void display()
{

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   gluLookAt(eye_position.x, eye_position.y, eye_position.z,
             eye_position.x + look.x, eye_position.y + look.y, eye_position.z + look.z,
             up.x, up.y, up.z);

   glRotatef(angleRotationZ, 0, 0, 1);

   drawPyramids();
   drawSpheres();
   drawCylinders();

   glutSwapBuffers(); // Render now
}

void init()
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(90, 1, 1, 5000.0);
}

void rotate(Point &a, Point &b, double angle)
{
   a = a * cos(angle) + b * sin(angle);
   b = b * cos(angle) - a * sin(angle);
}

void keyboardListener(unsigned char key, int x, int y)
{
   switch (key)
   {
   case '1':
      rotate(r, look, rotation_angle);
      break;

   case '2':
      rotate(r, look, -rotation_angle);
      break;

   case '3':
      rotate(look, up, rotation_angle);
      break;

   case '4':
      rotate(look, up, -rotation_angle);
      break;

   case '5':
      rotate(up, r, rotation_angle);
      break;

   case '6':
      rotate(up, r, -rotation_angle);
      break;

   case 'w':
      eye_position.z += 0.3;
      break;
   case 's':
      eye_position.z -= 0.3;
      break;

   case 'a':
      angleRotationZ += 5.0f;
      break;

   case 'd':
      angleRotationZ -= 5.0f;
      break;

   case ',':
      triangleLength -= 0.1;
      sphere_radius += (maxSphereRadius / 16.0);
      triangleLength = triangleLength < 0 ? 0 : triangleLength;
      if (triangleLength == 0)
      {
         sphere_radius = maxSphereRadius;
      }
      break;

   case '.':
      triangleLength += 0.1;
      sphere_radius -= (maxSphereRadius / 16.0);
      triangleLength = triangleLength > maxTriangleLength ? maxTriangleLength : triangleLength;

      if (triangleLength == maxTriangleLength)
      {
         sphere_radius = 0;
      }
      break;

   case 'q':
      exit(0);
      break;
   default:
      break;
   }

   glutPostRedisplay();
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x, int y)
{
   switch (key)
   {
   case GLUT_KEY_UP: // down arrow key
      eye_position.x += 0.3;
      break;
   case GLUT_KEY_DOWN: // up arrow key
      eye_position.x -= 0.3;
      break;

   case GLUT_KEY_RIGHT:
      eye_position.y += 0.3;
      break;
   case GLUT_KEY_LEFT:
      eye_position.y -= 0.3;
      break;

   case GLUT_KEY_PAGE_UP:
      eye_position.z += 0.3;
      break;
   case GLUT_KEY_PAGE_DOWN:
      eye_position.z -= 0.3;
      break;

   default:
      break;
   }
   glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char **argv)
{

   glutInit(&argc, argv);                                    // Initialize GLUT
   glutInitWindowSize(840, 840);                             // Set the window's initial width & height
   glutInitWindowPosition(50, 50);                           // Position the window's initial top-left corner
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color
   glutCreateWindow("Magic Cube");                           // Create a window with the given title
   glutDisplayFunc(display);                                 // Register display callback handler for window re-paint
   init();
   glutKeyboardFunc(keyboardListener);  // Register callback handler for normal-key event
   glutSpecialFunc(specialKeyListener); // Register callback handler for special-key event
   initGL();                            // Our own OpenGL initialization
   glutMainLoop();                      // Enter the event-processing loop
   return 0;
}
