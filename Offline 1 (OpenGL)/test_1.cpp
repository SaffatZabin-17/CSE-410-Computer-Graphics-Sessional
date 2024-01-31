#include <bits/stdc++.h>

using namespace std;

#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include <windows.h>
    #include <glut.h>
#endif

int counter = 0;

void axes(){
    glBegin(GL_LINES);{
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-1, 0.0f);
        glVertex2f(1, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(0.0f, 1);
        glVertex2f(0.0f, -1);

    } glEnd();

}

void square(double a){
    glBegin(GL_QUADS);{
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(-a, -a);
        glVertex2f(a, -a);
        glVertex2f(a, a);
        glVertex2f(-a, a);
    } glEnd();
}

void display(){
    cout << "We are in display function" << endl;
    //cout << "Counter = " << counter++ << endl;
    glClear(GL_COLOR_BUFFER_BIT);

    axes();

    double position_x[] = {0.5, 0.5, -0.5, -0.5};
    double position_y[] = {0.5, -0.5, -0.5, 0.5};

    /*for(int i=0;i<4;i++){
        glPushMatrix();
        glRotatef(90 * i, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.5, 0.5, 0.0f);
        square(0.25);
        glPopMatrix();
    }*/

    glPushMatrix();
    glRotatef(counter % 360, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.5, 0.5, 0.0f);
    //square(0.25);
    glPopMatrix();

    glFlush();

}

void init(){
    //cout << "We are in init function" << endl;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque

    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
    glLoadIdentity(); // Reset the projection matrix
    gluPerspective(60.0f, 1.0f, 1.0f, 100.0f); // Set the perspective with angle of view = 45 degrees, width-to-height ratio, near z clipping coordinate, far z clipping coordinate
}

void idle(){
    cout << "We are in idle function" << endl;
    glutPostRedisplay();
}

void timer(int value){
    counter++;
    cout << "We are in timer function" << endl;
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}


int main(int argc, char** argv){
    cout << "Hello World!" << endl;
    glutInit(&argc, argv);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(720, 720);
    glutCreateWindow("OpenGL Setup Test");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(10, timer, 0);
    //glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}