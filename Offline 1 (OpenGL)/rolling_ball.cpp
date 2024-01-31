#include <GL/glut.h>

const int gridX = 8;
const int gridZ = 8;
const float tileSize = 1.0f;

// Initialize OpenGL Graphics
void initGL() {
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f); // Set background (clear) color to light gray
    glEnable(GL_DEPTH_TEST); // Enable depth testing for z-culling
}

// Function to draw a checkered floor
void drawCheckeredFloor() {
    glBegin(GL_QUADS);
    for (int x = -gridX; x < gridX; ++x) {
        for (int z = -gridZ; z < gridZ; ++z) {
            glColor3f((x + z) % 2 == 0 ? 0.1f : 1.0f, (x + z) % 2 == 0 ? 0.1f : 1.0f, (x + z) % 2 == 0 ? 0.1f : 1.0f);
            glVertex3f(x * tileSize, 0.0f, z * tileSize);
            glVertex3f(x * tileSize, 0.0f, (z + 1) * tileSize);
            glVertex3f((x + 1) * tileSize, 0.0f, (z + 1) * tileSize);
            glVertex3f((x + 1) * tileSize, 0.0f, z * tileSize);
        }
    }
    glEnd();
}

// Function to set the camera and perspective
void setCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)800 / (double)600, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5.0, 5.0, 5.0,  // Camera position
              0.0, 0.0, 0.0,  // Look at point
              0.0, 1.0, 0.0); // Up vector
}

// Callback function called to draw the 3D scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffer
    setCamera();

    drawCheckeredFloor(); // Draw the checkered floor

    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

// Main function: GLUT runs as a console application starting at main()
int main(int argc, char** argv) {
    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Enable double buffered mode with RGB colors and depth
    glutInitWindowSize(800, 600); // Set the window's initial width & height
    glutCreateWindow("OpenGL Checkered Floor"); // Create a window with the given title

    initGL(); // Our own OpenGL initialization
    glutDisplayFunc(display); // Register callback handler for window re-paint
    glutMainLoop(); // Enter the infinite event-processing loop
    return 0;
}
