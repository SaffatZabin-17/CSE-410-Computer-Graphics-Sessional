#include <bits/stdc++.h>
#include "1905060_classes.h"

#define rotation_angle pi / 180.0

int pixel, objectCount, lightSourceCount, spotLightSourceCount;
int windowWidth = 500, windowHeight = 500;
double viewAngle = 80;
int image_count = 11;

Point eye_position, up, r, look;


void drawScene()
{
    int i = 0;

    while(i < objects.size()){
        Object *temp = objects[i];
        temp->draw();
        i++;
    }

    i = 0;

    while(i < pointLights.size()){
        PointLight *temp = pointLights[i];
        temp->draw();
        i++;
    }
    
    i = 0;

    while(i < spotLights.size()){
        SpotLight *temp = spotLights[i];
        temp->draw();
        i++;
    }
}

void capture()
{
    double planeDistance, du, dv;
    Point topleft;
    int nearest;
    double t, tMin;
    Point ray_direction;
    double dummyT;

    bitmap_image image(pixel, pixel);
    image.set_all_channels(0, 0, 0);


    planeDistance = ((double)(windowHeight) / 2.0) / tan((viewAngle * pi) / (2.0 * 180.0));
    topleft = eye_position + (look * planeDistance) - (r * (windowWidth/2)) + (up * (windowHeight/2));

    du = (double)(windowWidth) / (double)(pixel);
    dv = (double)(windowHeight) / (double)(pixel);

    topleft = topleft + r * (0.5*du) - up*(0.5*dv); 

    int flag = 0;

    for (int i = 0; i < pixel; i++)
    {
        for (int j = 0; j < pixel; j++)
        {
            nearest = INT_MAX;
            tMin = INT_MAX;
            Point current_pixel = topleft + r * (du * j) - up * (dv * i);
            Ray ray(eye_position, current_pixel - eye_position);

            Color color (0, 0, 0);

            for (int k = 0; k < objects.size(); k++)
            {
                Object *temp = objects.at(k);
                t = temp->intersect(ray, &color, 0);
                if (t > 0 && (tMin - t) > epsilon)
                {
                    tMin = t;
                    nearest = k;
                }
            }
            if (nearest != INT_MAX)
            {
                objects[nearest]->intersect(ray, &color, 1);
                color.clip();
                image.set_pixel(j, i, color.red * 255, color.green * 255, color.blue * 255);
            }
        }
    }

    string name = "Output_" + to_string(image_count) + ".bmp";
    image.save_image(name);
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
    case '0':
        capture();
        image_count++;
        break;
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
    default:
        break;
    }

    glutPostRedisplay();
}

void specialKeyListener(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP: // down arrow key
        eye_position.x += 1;
        break;
    case GLUT_KEY_DOWN: // up arrow key
        eye_position.x -= 1;
        break;

    case GLUT_KEY_RIGHT:
        eye_position.y += 1;
        break;
    case GLUT_KEY_LEFT:
        eye_position.y -= 1;
        break;

    case GLUT_KEY_PAGE_UP:
        eye_position.z += 1;
        break;
    case GLUT_KEY_PAGE_DOWN:
        eye_position.z -= 1;
        break;

    default:
        break;
    }
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0); // color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    // initialize the matrix
    glLoadIdentity();

    gluLookAt(eye_position.x, eye_position.y, eye_position.z, 
              eye_position.x + look.x, eye_position.y + look.y, eye_position.z + look.z, 
              up.x, up.y, up.z);
    
    glMatrixMode(GL_MODELVIEW);
    //drawGrid();
    drawScene();
    glutSwapBuffers();
}

void init()
{
    eye_position.set_value(100, 100, 50);
    up.set_value(0, 0, 1);
    r.set_value((-1/sqrt(2)), (1/sqrt(2)), 0);
    look.set_value((-1/sqrt(2)), (-1/sqrt(2)), 0);
    
    // clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewAngle, 1, 1, 500.0);
}

void loadData()
{
    ifstream input("scene.txt");

    // floor object add
    Object *floor = new Floor(1000, 20);
    objects.push_back(floor);

    string object;
    Object *temp;
    PointLight *temp_PointLight;
    SpotLight *temp_SpotLight;

    input >> recursion;
    input >> pixel;

    input >> objectCount;

    for (int i = 0; i < objectCount; i++)
    {
        input >> object;
        if (object == "sphere")
        {
            double center_x, center_y, center_z;
            double radius;
            double color_red, color_green, color_blue;
            double ambient, diffuse, specular, reflection;
            double shininess;

            input >> center_x >> center_y >> center_z;
            input >> radius;
            input >> color_red >> color_green >> color_blue;
            input >> ambient >> diffuse >> specular >> reflection;
            input >> shininess;

            Point center(center_x, center_y, center_z);
            Color color(color_red, color_green, color_blue);

            temp = new Sphere(center, radius);
            temp->setColor(color);
            temp->setCoEfficients(ambient, diffuse, specular, reflection);
            temp->setShininess(shininess);

            objects.push_back(temp);
        }
        else if (object == "triangle")
        {
            double x1, y1, z1;
            double x2, y2, z2;
            double x3, y3, z3;
            double color_red, color_green, color_blue;
            double ambient, diffuse, specular, reflection;
            double shininess;

            input >> x1 >> y1 >> z1;
            input >> x2 >> y2 >> z2;
            input >> x3 >> y3 >> z3;
            input >> color_red >> color_green >> color_blue;
            input >> ambient >> diffuse >> specular >> reflection;
            input >> shininess;

            Point p1(x1, y1, z1), p2(x2, y2, z2), p3(x3, y3, z3);
            Color color(color_red, color_green, color_blue);

            temp = new Triangle(p1, p2, p3);

            temp->setColor(color);
            temp->setCoEfficients(ambient, diffuse, specular, reflection);
            temp->setShininess(shininess);

            objects.push_back(temp);
        }
        else if (object == "general")
        {
            double A, B, C, D, E, F, G, H, I, J;
            int ref_x, ref_y, ref_z, length, width, height;
            double color_red, color_green, color_blue;
            double ambient, diffuse, specular, reflection;
            double shininess;

            input >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
            input >> ref_x >> ref_y >> ref_z >> length >> width >> height;
            input >> color_red >> color_green >> color_blue;
            input >> ambient >> diffuse >> specular >> reflection;
            input >> shininess;

            temp = new General(A, B, C, D, E, F, G, H, I, J);
            Point ref_point(ref_x, ref_y, ref_z);
            Color color(color_red, color_green, color_blue);

            temp->setReferencePoint(ref_point);
            temp->setColor(color);
            temp->setCoEfficients(ambient, diffuse, specular, reflection);
            temp->setShininess(shininess);

            objects.push_back(temp);
        }
    }

    input >> lightSourceCount;

    for (int i = 0; i < lightSourceCount; i++)
    {
        double pos_x, pos_y, pos_z;
        double color_red, color_green, color_blue;

        input >> pos_x >> pos_y >> pos_z;
        input >> color_red >> color_green >> color_blue;

        Point position(pos_x, pos_y, pos_z);
        Color color(color_red, color_green, color_blue);

        temp_PointLight = new PointLight(position, color);

        pointLights.push_back(temp_PointLight);
    }

    input >> spotLightSourceCount;
    
    for (int i = 0; i < spotLightSourceCount; i++)
    {
        double pos_x, pos_y, pos_z;
        double color_red, color_green, color_blue;
        double dir_x, dir_y, dir_z;
        double cutoff_angle;

        input >> pos_x >> pos_y >> pos_z;
        input >> color_red >> color_green >> color_blue;
        input >> dir_x >> dir_y >> dir_z;
        input >> cutoff_angle;

        Point position(pos_x, pos_y, pos_z);
        Color color(color_red, color_green, color_blue);
        Point direction(dir_x, dir_y, dir_z);

        temp_SpotLight = new SpotLight();
        PointLight temp(position, color);
        
        temp_SpotLight->setPointLight(temp);
        temp_SpotLight->setLightDirection(direction);
        temp_SpotLight->setCutoffAngle(cutoff_angle);
        
        spotLights.push_back(temp_SpotLight);
    }
}

void clear_memory(){
    for (int i = 0; i <= objects.size(); i++)
    {
        delete[] objects[i];
    }
    for (int i = 0; i <= pointLights.size(); i++)
    {
        delete[] pointLights[i];
    }
    for (int i = 0; i <= spotLights.size(); i++)
    {
        delete[] spotLights[i];
    }

    objects.clear();
    objects.shrink_to_fit();

    pointLights.clear();
    pointLights.shrink_to_fit();

    spotLights.clear();
    spotLights.shrink_to_fit();
}

int main(int argc, char **argv)
{

    loadData();

    glutInit(&argc, argv);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color

    glutCreateWindow("1.exe");

    init();

    glEnable(GL_DEPTH_TEST); // enable Depth Testing

    glutDisplayFunc(display); // display callback function

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    glutMainLoop(); // The main loop of OpenGL

    clear_memory();

    return 0;
}
