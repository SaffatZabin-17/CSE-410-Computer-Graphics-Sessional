#include <GL/glut.h>
#include <math.h>

void drawSolidDisk(float radius, int segments) {
    // Draw filled circle in black
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for the disk
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f); // Center of the disk
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        glVertex3f(radius * cos(angle), 0.3f, radius * sin(angle));
    }
    glEnd();

    // Draw the perimeter of the circle in white
    glColor3f(1.0f, 1.0f, 1.0f); // White color for the perimeter
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        glVertex3f(radius * cos(angle), 0.3f, radius * sin(angle));
    }
    glEnd();
}

void drawLinesFromDisk(float radius, float line_length, int segments) {
    // Calculate the point on the disk perimeter
    float angle = 0.0f; // Change this angle to position the lines differently on the disk
    float x = radius * cos(angle);
    float z = radius * sin(angle);
    float y = 0.3f; // Height of the disk above the XY plane

    // Draw two white lines originating from the same point on the disk perimeter and going downwards
    glColor3f(1.0f, 1.0f, 1.0f); // White color for the lines
    glBegin(GL_LINES);
        // First line going down from the edge of the disk along the Y axis
        glVertex3f(x, y, z);
        glVertex3f(x, y - line_length, z);

        // Second line going down from the edge of the disk along the Y axis at a different angle
        // We can choose to keep the same x and z to make them go straight down
        //glVertex3f(x, y, z);
        //glVertex3f(x, y - line_length, z);
    glEnd();
}

void drawAxes()
{
    glBegin(GL_LINES);
    // X axis in red
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-3.0f, 0.0f, 0.0f);
    glVertex3f(3.0f, 0.0f, 0.0f);

    // Y axis in green
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, -3.0f, 0.0f);
    glVertex3f(0.0f, 3.0f, 0.0f);

    // Z axis in blue
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -3.0f);
    glVertex3f(0.0f, 0.0f, 3.0f);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set the camera
    gluLookAt(3.0, 3.0, 3.0,  // Camera in the 3D space
              0.0, 0.0, 0.0,  // Looking at the origin
              0.0, 1.0, 0.0); // Up vector

    drawAxes();
    glClear(GL_DEPTH_BUFFER_BIT);
    drawSolidDisk(1.0f, 36);
    drawLinesFromDisk(1.0f, 1.0f, 36);
    //drawFilledDisk(1.5f, 36, 0.0f, 0.0f, 0.0f);
    //drawDiskPerimeter(1.5f, 36, 1.0f, 1.0f, 1.0f);
    //drawDisk(1.5f, 36);
    glutSwapBuffers();
}

void init()
{
    // Set clear color to black and enable depth test
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);

    // Switch back to modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL 3D Axes");

    init(); // Call the init function

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
