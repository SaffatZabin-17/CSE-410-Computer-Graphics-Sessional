#include <bits/stdc++.h>
#include "bitmap_image.hpp"
#include <GL/glut.h>

using namespace std;

#define pi (2 * acos(0.0))

int recursion;
#define epsilon 0.0000001

struct Color
{
    double red, green, blue;
    Color()
    {
        red = green = blue = 0.0;
    }

    Color(double red, double green, double blue)
    {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    void setColor(double red, double green, double blue)
    {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    void clip()
    {
        this->red = (this->red > 1) ? 1 : (this->red < 0 ? 0 : this->red);
        this->green = (this->green > 1) ? 1 : (this->green < 0 ? 0 : this->green);
        this->blue = (this->blue > 1) ? 1 : (this->blue < 0 ? 0 : this->blue);
    }
};

class Point
{
public:
    double x, y, z;

    Point()
    {
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
        //this->n = 0.0;
    }

    Point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        //this->n = 0.1;
    }

    Point(double x, double y, double z, double n)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        //this->n = n;
    }

    Point operator+(const Point p)
    {
        Point temp;
        temp.x = this->x + p.x;
        temp.y = this->y + p.y;
        temp.z = this->z + p.z;
        //temp.n = 0.1;
        return temp;
    }

    Point operator-(const Point p)
    {
        Point temp;
        temp.x = this->x - p.x;
        temp.y = this->y - p.y;
        temp.z = this->z - p.z;
        //temp.n = 0.1;
        return temp;
    }

    Point operator*(const double c)
    {
        Point temp;
        temp.x = this->x * c;
        temp.y = this->y * c;
        temp.z = this->z * c;
        //temp.n = 0.1;
        return temp;
    }

    Point operator/(const double c)
    {
        Point temp;
        temp.x = this->x / c;
        temp.y = this->y / c;
        temp.z = this->z / c;
        //temp.n = 0.1;
        return temp;
    }

    double operator*(const Point &p)
    {
        Point temp;
        temp.x = this->x * p.x;
        temp.y = this->y * p.y;
        temp.z = this->z * p.z;
        //temp.n = 0.1;
        return temp.x + temp.y + temp.z;
    }

    Point operator^(const Point &p)
    {
        Point temp;
        temp.x = this->y * p.z - this->z * p.y;
        temp.y = this->z * p.x - this->x * p.z;
        temp.z = this->x * p.y - this->y * p.x;
        //temp.n = 0.1;
        return temp;
    }

    void set_value(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        //this->n = 0.1;
    }

    void normalize()
    {
        double val = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
        this->x = this->x / val;
        this->y = this->y / val;
        this->z = this->z / val;
    }

    double distanceCalc(Point p)
    {
        double val = sqrt(pow(this->x - p.x , 2) + pow(this->y - p.y , 2) + pow(this->z - p.z , 2));
        return val;
    }
};

class Ray
{

public:
    Point start;
    Point direction;
    Ray()
    {
        this->start.x = 0.0;
        this->start.y = 0.0;
        this->start.z = 0.0;
        this->direction.x = 0.0;
        this->direction.y = 0.0;
        this->direction.z = 0.0;
    }

    Ray(Point start, Point direction)
    {
        this->start = start;
        direction.normalize();
        this->direction = direction;
    }
};

class PointLight
{

public:
    Point light_position;
    Color color;
    PointLight()
    {
        this->light_position.x = 0.0;
        this->light_position.y = 0.0;
        this->light_position.z = 0.0;
        this->color.red = 0.0;
        this->color.green = 0.0;
        this->color.blue = 0.0;
    }

    PointLight(Point light_position, Color color)
    {
        this->light_position.x = light_position.x;
        this->light_position.y = light_position.y;
        this->light_position.z = light_position.z;
        this->color.red = color.red;
        this->color.green = color.green;
        this->color.blue = color.blue;
    }

    void draw()
    {

        glColor3f(color.red, color.green, color.blue);
        glPointSize(3);
        glBegin(GL_POINTS);
        {
            glVertex3f(light_position.x, light_position.y, light_position.z);
        }
        glEnd();
    }
};

vector<PointLight *> pointLights;

class SpotLight
{

public:
    PointLight pointLight;
    Point lightDirection;
    double cutOffAngle;

    SpotLight() {}

    void setPointLight(PointLight pointLight)
    {
        this->pointLight.light_position.x = pointLight.light_position.x;
        this->pointLight.light_position.y = pointLight.light_position.y;
        this->pointLight.light_position.z = pointLight.light_position.z;
        this->pointLight.color.red = pointLight.color.red;
        this->pointLight.color.green = pointLight.color.green;
        this->pointLight.color.blue = pointLight.color.blue;
    }

    void setLightDirection(Point direction)
    {
        this->lightDirection.x = direction.x;
        this->lightDirection.y = direction.y;
        this->lightDirection.z = direction.z;
    }

    void setCutoffAngle(double cutoffAngle)
    {
        this->cutOffAngle = cutOffAngle;
    }

    void draw()
    {
        glColor3f(pointLight.color.red, pointLight.color.green, pointLight.color.blue);
        glPointSize(10);
        glBegin(GL_POINTS);
        {
            glVertex3f(pointLight.light_position.x, pointLight.light_position.y, pointLight.light_position.z);
        }
        glEnd();
    }
};

vector<SpotLight *> spotLights;

class Object
{

public:
    Point referencePoint;
    double height, width, length;
    Color color;
    double coEfficients[4]; // ambient, diffuse, specular, reflection coefficients
    int shininess;          // exponent term of specular component

    Object() {}
    virtual void draw() {}
    virtual void testPrint() {}
    virtual double intersect(Ray r, Color *color, int level)
    {
        return -1.0;
    }
    void setColor(Color color)
    {
        this->color.red = color.red;
        this->color.green = color.green;
        this->color.blue = color.blue;
    }

    void setShininess(int shininess)
    {
        this->shininess = shininess;
    }

    void setReferencePoint(Point p)
    {
        this->referencePoint.x = p.x;
        this->referencePoint.y = p.y;
        this->referencePoint.z = p.z;
    }

    void setCoEfficients(double ambient, double diffuse, double specular, double reflection)
    {
        this->coEfficients[0] = ambient;
        this->coEfficients[1] = diffuse;
        this->coEfficients[2] = specular;
        this->coEfficients[3] = reflection;
    }
};

vector<Object *> objects;

class Floor : public Object
{

public:
    double tileWidth, floorWidth;
    Floor(double floodWidth, double tileWidth)
    {
        this->floorWidth = floodWidth;
        this->tileWidth = tileWidth;
        Point reference_point(-floodWidth / 2, -floodWidth / 2, 0);
        this->setReferencePoint(reference_point);

        length = tileWidth;

        this->setCoEfficients(0.4, 0.2, 0.2, 0.2);
        this->setShininess(10);
    }
    void draw()
    {

        int loop = floorWidth / tileWidth;

        int i = -loop / 2, j;

        while (i <= loop / 2)
        {
            j = -loop / 2;
            while (j <= loop / 2)
            {
                if ((i + j) % 2 != 0)
                {
                    glColor3f(1, 1, 1);
                }
                else
                {
                    glColor3f(0, 0, 0);
                }
                glBegin(GL_QUADS);
                {
                    glVertex3f(i * tileWidth, j * tileWidth, 0);
                    glVertex3f(i * tileWidth, (j + 1) * tileWidth, 0);
                    glVertex3f((i + 1) * tileWidth, (j + 1) * tileWidth, 0);
                    glVertex3f((i + 1) * tileWidth, j * tileWidth, 0);
                }
                glEnd();

                j++;
            }
            i++;
        }
    }

    double intersect(Ray r, Color *c, int level)
    {

        double tmin;
        Point normalToPlane(0, 0, 1); // this is the normal of xy plane... point(0,0,0)

        // cout << "Dot product 1 : " << referencePoint.dotProduct(r.direction, normalToPlane);
        // cout << "Dot product 2 : " <<  r.direction * normalToPlane << endl;
        if (r.direction * normalToPlane == 0)
        {
            return -1;
        }

        tmin = (-1) * ((r.start * normalToPlane) / (r.direction * normalToPlane));

        // calculate if the intersection point is on the plane
        Point intersectionPoint = r.start + (r.direction * tmin);
        if (!((intersectionPoint.x > referencePoint.x) && (intersectionPoint.x < -referencePoint.x)))
        {
            return -1;
        }

        if (!((intersectionPoint.y > referencePoint.y) && (intersectionPoint.y < -referencePoint.y)))
        {
            return -1;
        }
        if (level == 0)
        {
            return tmin;
        }

        // other values of level

        // calculate intersection point
        int xx = ceil(intersectionPoint.x / length);
        int yy = ceil(intersectionPoint.y / length);
        Color intersectionPointColor;
        if ((xx + yy) % 2 == 0)
        {
            intersectionPointColor.setColor(0, 0, 0);
        }
        else
        {
            intersectionPointColor.setColor(1, 1, 1);
        }

        double red_component = intersectionPointColor.red * this->coEfficients[0];
        double green_component = intersectionPointColor.green * this->coEfficients[0];
        double blue_component = intersectionPointColor.blue * this->coEfficients[0];
        c->setColor(red_component, green_component, blue_component);

        // calculate normal
        Ray normal(intersectionPoint, normalToPlane);

        double tempT, tempTmin, tRef, tMinRef;
        int nearest;

        // pointLight
        int i = 0;
        while (i < pointLights.size())
        {
            PointLight *tempPlLight = pointLights[i];
            Ray rayL(tempPlLight->light_position, (tempPlLight->light_position - intersectionPoint));

            tempTmin = INT_MAX;

            int j = 0;

            while (j < objects.size())
            {
                Object *tempObj = objects.at(j);
                tempT = tempObj->intersect(rayL, c, 0);
                if (tempT > 0 && tempT < tempTmin)
                {
                    tempTmin = tempT;
                }
                j++;
            }

            // calculate shadow point
            Point shadowPoint = rayL.start + (rayL.direction * tempTmin);

            double Intersection_distance, distShadow;

            Intersection_distance = rayL.start.distanceCalc(intersectionPoint);
            distShadow = rayL.start.distanceCalc(shadowPoint);

            if (distShadow < Intersection_distance)
            {
                continue;
            }

            double dot_product = rayL.direction * normal.direction;
            double lambertValue = max(0.0, dot_product);

            double red_component = c->red + tempPlLight->color.red * this->coEfficients[1] * lambertValue * intersectionPointColor.red;
            double green_component = c->green + tempPlLight->color.green * this->coEfficients[1] * lambertValue * intersectionPointColor.green;
            double blue_component = c->blue + tempPlLight->color.blue * this->coEfficients[1] * lambertValue * intersectionPointColor.blue;

            c->setColor(red_component, green_component, blue_component);

            Point rayR_direction = (normal.direction * (rayL.direction * normal.direction) * 2) - rayL.direction;

            Ray rayR(intersectionPoint, rayR_direction);
            Point V = r.direction * -1;

            double phongValue = max(rayR.direction * V, 0.0);

            red_component = c->red + tempPlLight->color.red * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.red;
            green_component = c->green + tempPlLight->color.green * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.green;
            blue_component = c->blue + tempPlLight->color.blue * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.blue;

            i++;
        }

        for (int i = 0; i < spotLights.size(); i++)
        {
            SpotLight *tempSpotLight = spotLights[i];
            Ray rayL(tempSpotLight->pointLight.light_position, (tempSpotLight->pointLight.light_position - intersectionPoint));
            Point SpotLightDirection = tempSpotLight->lightDirection;
            SpotLightDirection.normalize();

            double cosVal = SpotLightDirection * (rayL.direction * -1); // referencePoint.dotProduct(SpotLightDirection, referencePoint.multiply(rayL.direction, -1));
            double angle = (acos(cosVal) * 180) / pi;

            if (angle > tempSpotLight->cutOffAngle)
            {
                continue;
            }

            tempTmin = INT_MAX;

            int j = 0;

            while (j < objects.size())
            {
                Color *dummyColor = new Color();
                Object *tempObj = objects[i];
                tempT = tempObj->intersect(rayL, dummyColor, 0);
                if (tempT > 0 && tempT < tempTmin)
                {
                    tempTmin = tempT;
                }
                j++;
            }

            // calculate shadow point
            // Point shadowPoint = referencePoint.plusPoint(rayL.start, referencePoint.multiply(rayL.direction, tempTmin));

            Point shadowPoint = rayL.start + (rayL.direction * tempTmin);

            double distIntersect, distShadow;

            distIntersect = rayL.start.distanceCalc(intersectionPoint);
            distShadow = rayL.start.distanceCalc(shadowPoint);
            if (distShadow < distIntersect)
            {
                continue;
            }

            // calculate coeff

            // color += pl.color*coEfficient[DIFF]*lambertValue*intersectionPointColor

            double lambertValue = max(0.0, rayL.direction * normal.direction);

            double red_component = c->red + tempSpotLight->pointLight.color.red * this->coEfficients[1] * lambertValue * intersectionPointColor.red;
            double green_component = c->green + tempSpotLight->pointLight.color.green * this->coEfficients[1] * lambertValue * intersectionPointColor.green;
            double blue_component = c->blue + tempSpotLight->pointLight.color.blue * this->coEfficients[1] * lambertValue * intersectionPointColor.blue;

            c->setColor(red_component, green_component, blue_component);

            Point rayR_direction = (normal.direction * (rayL.direction * normal.direction) * 2) - rayL.direction;

            Ray rayR(intersectionPoint, rayR_direction);
            Point V = r.direction * -1;

            double phongValue = max(rayR.direction * V, 0.0);

            red_component = c->red + tempSpotLight->pointLight.color.red * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.red;
            green_component = c->green + tempSpotLight->pointLight.color.green * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.green;
            blue_component = c->blue + tempSpotLight->pointLight.color.blue * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.blue;

            c->setColor(red_component, green_component, blue_component);
        }

        // reflection part

        if (level >= recursion)
        {
            return tmin;
        }

        // calculate reflected ray
        // Point rayReflection_direction = referencePoint.minusPoint(r.direction, referencePoint.multiply(normal.direction, (referencePoint.dotProduct(r.direction, normal.direction)) * 2));
        Point rayReflection_direction = r.direction - (normal.direction * ((r.direction * normal.direction) * 2));
        Ray ray_reflected(intersectionPoint, rayReflection_direction);

        ray_reflected.start = ray_reflected.start + (rayReflection_direction * epsilon);

        // find tmin from the nearest intersecting object,
        nearest = tMinRef = INT_MAX;
        // tMinRef = INT_MAX;

        for (int i = 0; i < objects.size(); i++)
        {
            Object *tempObj = objects[i];
            tRef = tempObj->intersect(ray_reflected, c, 0);

            if (tRef > 0 && tRef < tMinRef)
            {
                tMinRef = tRef;
                nearest = i;
            }
        }

        Color *reflectColor = new Color;
        double dummyT;
        if (nearest != INT_MAX)
        {
            Object *tempMin = objects[nearest];
            dummyT = tempMin->intersect(ray_reflected, reflectColor, level + 1);

            double red_component = c->red + reflectColor->red * this->coEfficients[3];
            double green_component = c->green + reflectColor->green * this->coEfficients[3];
            double blue_component = c->blue + reflectColor->blue * this->coEfficients[3];
            c->setColor(red_component, green_component, blue_component);
        }

        return tmin;
    }
};

class Sphere : public Object
{

public:
    Sphere(Point center, double radius)
    {
        referencePoint = center;
        length = radius;
    }

    Point mirror(const Point p)
    {
        Point temp;

        temp.x = p.x;
        temp.y = p.y;
        temp.z = -(p.z);

        return temp;
    }

    void draw()
    {
        Point points[100][100];
        int slices = 20;
        int stacks = 20;
        glPushMatrix();
        glTranslatef(referencePoint.x, referencePoint.y, referencePoint.z);
        // generate points
        for (int i = 0; i <= stacks; i++)
        {
            for (int j = 0; j <= slices; j++)
            {
                points[i][j].x = (length * cos((double)i / (double)stacks * (pi / 2))) * cos(((double)j / (double)slices) * 2 * pi);
                points[i][j].y = (length * cos((double)i / (double)stacks * (pi / 2))) * sin(((double)j / (double)slices) * 2 * pi);
                points[i][j].z = (length * sin((double)i / (double)stacks * (pi / 2)));
            }
        }
        // draw quads using generated points
        for (int i = 0; i < stacks; i++)
        {
            glColor3f(color.red, color.green, color.blue);

            for (int j = 0; j < slices; j++)
            {
                glBegin(GL_QUADS);
                {
                    // upper hemisphere
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                    // lower hemisphere
                    glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
                }
                glEnd();
            }
        }
        glPopMatrix();
    }

    double intersect(Ray r, Color *c, int level)
    {
        double ld, d, ll, t1, t2, tmin;
        double tempT, tempTmin, tRef, tMinRef;
        int nearest;

        // shift the start of ray to center
        r.start = r.start - referencePoint;

        ld = r.direction * (r.start * -1);

        bool flag = ld < 0 ? 1 : 0;

        if (flag)
        {
            return -1;
        }

        double Ro2 = pow(r.start.x, 2) + pow(r.start.y, 2) + pow(r.start.z, 2);

        d = sqrt(Ro2 - (pow(ld, 2)));

        flag = pow(d, 2) > pow(length, 2) ? 1 : 0;

        if (flag)
        {
            return -1.0;
        }

        ll = sqrt(length * length - d * d);
        t1 = ld + ll;
        t2 = ld - ll;
        tmin = (t1 >= t2) ? t2 : t1;

        flag = (level == 0) ? 1 : 0;
        if (flag)
        {
            return tmin;
        }

        // other values of level

        r.start = r.start + referencePoint;

        // calculate intersection point
        Point intersectionPoint = r.start + (r.direction * tmin);
        Color intersectionPointColor;

        intersectionPointColor.setColor(this->color.red, this->color.green, this->color.blue);

        double red_component = intersectionPointColor.red * this->coEfficients[0];
        double green_component = intersectionPointColor.green * this->coEfficients[0];
        double blue_component = intersectionPointColor.blue * this->coEfficients[0];

        c->setColor(red_component, green_component, blue_component);

        // calculate normal
        Point normalDir = intersectionPoint - referencePoint;
        Ray normal(intersectionPoint, normalDir);

        // pointLight
        for (int i = 0; i < pointLights.size(); i++)
        {
            PointLight *temp_PointLight = pointLights[i];
            Ray rayL(temp_PointLight->light_position, temp_PointLight->light_position - intersectionPoint);

            tempTmin = INT_MAX;

            for (int j = 0; j < objects.size(); j++)
            {
                Color *dummyColor = new Color();
                Object *tempObj = objects[i];
                tempT = tempObj->intersect(rayL, dummyColor, 0);
                bool flag = (tempT > 0 && tempT < tempTmin) ? 1 : 0;
                if (flag)
                {
                    tempTmin = tempT;
                }
            }

            // calculate shadow point
            Point shadowPoint = rayL.start + (rayL.direction * tempTmin);

            double distIntersect, distShadow;

            distIntersect = rayL.start.distanceCalc(intersectionPoint);
            distShadow = rayL.start.distanceCalc(shadowPoint);
            flag = (distShadow < distIntersect) ? 1 : 0;
            if (flag)
            {
                continue;
            }

            // calculate coeff

            double lambertValue = max(0.0, rayL.direction * normal.direction);
            double red_component, green_component, blue_component;
            red_component = c->red + temp_PointLight->color.red * this->coEfficients[1] * lambertValue * intersectionPointColor.red;
            green_component = c->green + temp_PointLight->color.green * this->coEfficients[1] * lambertValue * intersectionPointColor.green;
            blue_component = c->blue + temp_PointLight->color.blue * this->coEfficients[1] * lambertValue * intersectionPointColor.blue;

            c->setColor(red_component, green_component, blue_component);

            Point rayR_direction = (normal.direction * ((rayL.direction * normal.direction) * 2)) - rayL.direction;
            Ray rayR(intersectionPoint, rayR_direction);

            Point V = r.direction * -1;

            double phongValue = max((rayR.direction * V), 0.0);

            red_component = c->red + temp_PointLight->color.red * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.red;
            green_component = c->green + temp_PointLight->color.green * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.green;
            blue_component = c->blue + temp_PointLight->color.blue * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.blue;

            c->setColor(red_component, green_component, blue_component);
        }

        // spotlight
        for (int i = 0; i < spotLights.size(); i++)
        {
            SpotLight *temp_SpotLight = spotLights[i];
            Ray rayL(temp_SpotLight->pointLight.light_position, (temp_SpotLight->pointLight.light_position - intersectionPoint));
            Point SpotLight_direction = temp_SpotLight->lightDirection;
            SpotLight_direction.normalize();

            double cosVal = (SpotLight_direction * (rayL.direction * -1));
            double angle = (acos(cosVal) * 180) / pi;

            bool flag = angle > temp_SpotLight->cutOffAngle ? 1 : 0;

            if (flag)
            {
                continue;
            }

            tempTmin = INT_MAX;

            for (int j = 0; j < objects.size(); j++)
            {
                Color dummyColor;
                Object *tempObj = objects[i];
                tempT = tempObj->intersect(rayL, &dummyColor, 0);
                bool flag = (tempT > 0 && tempT < tempTmin) ? 1 : 0;
                if (flag)
                {
                    tempTmin = tempT;
                }
            }

            // calculate shadow point
            Point shadowPoint = (rayL.start + (rayL.direction * tempTmin));

            double distIntersect, distShadow;

            distIntersect = rayL.start.distanceCalc(intersectionPoint);
            distShadow = rayL.start.distanceCalc(shadowPoint);
            flag = (distShadow < distIntersect) ? 1 : 0;
            if (flag)
            {
                continue;
            }

            // calculate coeff

            // color += pl.color*coEfficient[DIFF]*lambertValue*intersectionPointColor

            double lambertValue = max(0.0, (rayL.direction * normal.direction));

            double red_component, green_component, blue_component;

            red_component = c->red + temp_SpotLight->pointLight.color.red * this->coEfficients[1] * lambertValue * intersectionPointColor.red;
            green_component = c->green + temp_SpotLight->pointLight.color.green * this->coEfficients[1] * lambertValue * intersectionPointColor.green;
            blue_component = c->blue + temp_SpotLight->pointLight.color.blue * this->coEfficients[1] * lambertValue * intersectionPointColor.blue;

            c->setColor(red_component, green_component, blue_component);

            Point rayR_direction = (normal.direction * ((rayL.direction * normal.direction) * 2)) - rayL.direction;
            Ray rayR(intersectionPoint, rayR_direction);
            Point V = (r.direction * -1);

            double phongValue = max((rayR.direction * V), 0.0);

            red_component = c->red + temp_SpotLight->pointLight.color.red * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.red;
            green_component = c->green + temp_SpotLight->pointLight.color.green * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.green;
            blue_component = c->blue + temp_SpotLight->pointLight.color.blue * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.blue;

            c->setColor(red_component, green_component, blue_component);
        }

        // reflection part

        if (level >= recursion)
        {
            return tmin;
        }

        // calculate reflected ray

        Point rayReflection_direction = (r.direction - (normal.direction * ((r.direction * normal.direction) * 2)));
        Ray rayReflect(intersectionPoint, rayReflection_direction);

        rayReflect.start = (rayReflect.start + (rayReflect.direction * epsilon));

        // find tmin from the nearest intersecting object,
        nearest = INT_MAX;
        tMinRef = INT_MAX;

        for (int i = 0; i < objects.size(); i++)
        {
            Color dummyColor;
            Object *tempObj = objects[i];
            tRef = tempObj->intersect(rayReflect, &dummyColor, 0);

            bool flag = tRef > 0 && tRef < tMinRef;

            if (flag)
            {
                tMinRef = tRef;
                nearest = i;
            }
        }

        Color *reflectColor = new Color;
        double dummyT;
        if (nearest != INT_MAX)
        {
            Object *tempMin = objects[nearest];
            dummyT = tempMin->intersect(rayReflect, reflectColor, level + 1);

            double red_component, green_component, blue_component;

            red_component = c->red + reflectColor->red * this->coEfficients[3];
            green_component = c->green + reflectColor->green * this->coEfficients[3];
            blue_component = c->blue + reflectColor->blue * this->coEfficients[3];

            c->setColor(red_component, green_component, blue_component);
        }

        return tmin;
    }
};

class Triangle : public Object
{

public:
    Point p1, p2, p3;

    Triangle(Point p1, Point p2, Point p3)
    {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
    }

    void draw()
    {
        // cout <<"In triangle's draw"<<endl;
        glColor3f(color.red, color.green, color.blue);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(p1.x, p1.y, p1.z);
            glVertex3f(p2.x, p2.y, p2.z);
            glVertex3f(p3.x, p3.y, p3.z);
        }
        glEnd();
    }

    double calculateDeterminant(const std::vector<std::vector<double>> &matrix)
    {
        int dimension = matrix.size();
        // Base case: if the matrix is 1x1, return the single element
        if (dimension == 1)
        {
            return matrix[0][0];
        }
        // Base case: for a 2x2 matrix, return ad-bc
        if (dimension == 2)
        {
            return (matrix[0][0] * matrix[1][1]) - (matrix[1][0] * matrix[0][1]);
        }
        double determinant = 0.0;
        int sign = 1; // sign changes for each term
        for (int i = 0; i < dimension; i++)
        {
            // Generate smaller matrix by excluding current row and column
            std::vector<std::vector<double>> minorMatrix(dimension - 1, std::vector<double>(dimension - 1));
            for (int j = 1; j < dimension; j++)
            {
                int colIndex = 0;
                for (int k = 0; k < dimension; k++)
                {
                    if (k == i)
                    {
                        continue; // Skip the current column
                    }
                    minorMatrix[j - 1][colIndex] = matrix[j][k];
                    colIndex++;
                }
            }
            // Recursive call to find the determinant of the minor matrix
            determinant += sign * matrix[0][i] * calculateDeterminant(minorMatrix);
            sign = -sign; // Flip the sign for the next term
        }
        return determinant;
    }

    double determinant(double matrix[3][3])
    {
        double a = matrix[0][0];
        double b = matrix[0][1];
        double c = matrix[0][2];
        double d = matrix[1][0];
        double e = matrix[1][1];
        double f = matrix[1][2];
        double g = matrix[2][0];
        double h = matrix[2][1];
        double i = matrix[2][2];

        double determinant = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
        return determinant;
    }

    double intersect(Ray r, Color *c, int level)
    {

        // beta matrix
        double beta[3][3] = {
            {(p1.x - r.start.x), (p1.x - p3.x), r.direction.x},
            {(p1.y - r.start.y), (p1.y - p3.y), r.direction.y},
            {(p1.z - r.start.z), (p1.z - p3.z), r.direction.z}};

        // gamma matrix
        double gamma[3][3] = {
            {(p1.x - p2.x), (p1.x - r.start.x), r.direction.x},
            {(p1.y - p2.y), (p1.y - r.start.y), r.direction.y},
            {(p1.z - p2.z), (p1.z - r.start.z), r.direction.z}};

        // t matrix
        double tMat[3][3] = {
            {(p1.x - p2.x), (p1.x - p3.x), (p1.x - r.start.x)},
            {(p1.y - p2.y), (p1.y - p3.y), (p1.y - r.start.y)},
            {(p1.z - p2.z), (p1.z - p3.z), (p1.z - r.start.z)}};

        // base matrix A
        double A[3][3] = {
            {(p1.x - p2.x), (p1.x - p3.x), r.direction.x},
            {(p1.y - p2.y), (p1.y - p3.y), r.direction.y},
            {(p1.z - p2.z), (p1.z - p3.z), r.direction.z}};

        double detBeta = determinant(beta);
        double detGamma = determinant(gamma);
        double detT = determinant(tMat);
        double detA = determinant(A);
        double tmin;

        if (detA == 0)
        {
            return -1;
        }
        else
        {
            detBeta /= detA;
            detGamma /= detA;
            detT /= detA;
            // β + γ < 1   &   β > 0   &   γ > 0  & t > 0
            bool flag = ((detBeta + detGamma) < 1) && (detBeta > 0) && (detGamma > 0) && (detT > 0) ? 1 : 0;

            if (flag)
            {
                tmin = detT;
            }
            else
            {
                return -1;
            }
        }

        if (level == 0)
        {
            return tmin;
        }

        // dummy reference point
        referencePoint.set_value(0.0, 0.0, 0.0);

        // calculate intersection point
        Point intersectionPoint = (r.start + (r.direction * tmin));
        Color intersectionPointColor;
        intersectionPointColor.setColor(this->color.red, this->color.green, this->color.blue);

        // color = intersectionPointColor*coEfficient[AMB]

        double red_component = intersectionPointColor.red * this->coEfficients[0];
        double green_component = intersectionPointColor.green * this->coEfficients[0];
        double blue_component = intersectionPointColor.blue * this->coEfficients[0];

        c->setColor(red_component, green_component, blue_component);

        // calculate normal
        Point normalDir = (p2 - p1) ^ (p3 - p1); // referencePoint.crossProduct(referencePoint.minusPoint(p2, p1), referencePoint.minusPoint(p3, p1));
        normalDir.normalize();
        Ray normal;
        bool flag = (r.direction * -1) * normalDir > 0 ? 1 : 0;
        if (flag)
        {
            normal.start = intersectionPoint;
            normal.direction = normalDir;
        }
        else
        {
            normal.start = intersectionPoint;
            normal.direction = (normalDir * -1);
        }

        double tempT, tempTmin, tRef, tMinRef;
        int nearest;

        // politlight
        for (int i = 0; i < pointLights.size(); i++)
        {
            PointLight *temp_PointLight = pointLights[i];
            Ray rayL(temp_PointLight->light_position, temp_PointLight->light_position - intersectionPoint);

            tempTmin = INT_MAX;

            for (int j = 0; j < objects.size(); j++)
            {
                Color *dummyColor = new Color();
                Object *tempObj = objects[i];
                tempT = tempObj->intersect(rayL, dummyColor, 0);
                bool flag = (tempT > 0 && tempT < tempTmin) ? 1 : 0;
                if (flag)
                {
                    tempTmin = tempT;
                }
            }

            // calculate shadow point
            Point shadowPoint = rayL.start + (rayL.direction * tempTmin);

            double distIntersect, distShadow;

            distIntersect = rayL.start.distanceCalc(intersectionPoint);
            distShadow = rayL.start.distanceCalc(shadowPoint);
            flag = (distShadow < distIntersect) ? 1 : 0;
            if (flag)
            {
                continue;
            }

            // calculate coeff

            double lambertValue = max(0.0, rayL.direction * normal.direction);
            double red_component, green_component, blue_component;
            red_component = c->red + temp_PointLight->color.red * this->coEfficients[1] * lambertValue * intersectionPointColor.red;
            green_component = c->green + temp_PointLight->color.green * this->coEfficients[1] * lambertValue * intersectionPointColor.green;
            blue_component = c->blue + temp_PointLight->color.blue * this->coEfficients[1] * lambertValue * intersectionPointColor.blue;

            c->setColor(red_component, green_component, blue_component);

            Point rayR_direction = (normal.direction * ((rayL.direction * normal.direction) * 2)) - rayL.direction;
            Ray rayR(intersectionPoint, rayR_direction);

            Point V = r.direction * -1;

            double phongValue = max((rayR.direction * V), 0.0);

            red_component = c->red + temp_PointLight->color.red * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.red;
            green_component = c->green + temp_PointLight->color.green * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.green;
            blue_component = c->blue + temp_PointLight->color.blue * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.blue;

            c->setColor(red_component, green_component, blue_component);
        }

        // spotlight
        for (int i = 0; i < spotLights.size(); i++)
        {

            SpotLight *temp_SpotLight = spotLights[i];
            Ray rayL(temp_SpotLight->pointLight.light_position, (temp_SpotLight->pointLight.light_position - intersectionPoint));
            Point SpotLight_direction = temp_SpotLight->lightDirection;
            SpotLight_direction.normalize();

            double cosVal = (SpotLight_direction * (rayL.direction * -1));
            double angle = (acos(cosVal) * 180) / pi;

            bool flag = angle > temp_SpotLight->cutOffAngle ? 1 : 0;

            if (flag)
            {
                continue;
            }

            tempTmin = INT_MAX;

            for (int j = 0; j < objects.size(); j++)
            {
                Color dummyColor;
                Object *tempObj = objects[i];
                tempT = tempObj->intersect(rayL, &dummyColor, 0);
                bool flag = (tempT > 0 && tempT < tempTmin) ? 1 : 0;
                if (flag)
                {
                    tempTmin = tempT;
                }
            }

            // calculate shadow point
            Point shadowPoint = (rayL.start + (rayL.direction * tempTmin));

            double distIntersect, distShadow;

            distIntersect = rayL.start.distanceCalc(intersectionPoint);
            distShadow = rayL.start.distanceCalc(shadowPoint);
            flag = (distShadow < distIntersect) ? 1 : 0;
            if (flag)
            {
                continue;
            }

            // calculate coeff

            // color += pl.color*coEfficient[DIFF]*lambertValue*intersectionPointColor

            double lambertValue = max(0.0, (rayL.direction * normal.direction));

            double red_component, green_component, blue_component;

            red_component = c->red + temp_SpotLight->pointLight.color.red * this->coEfficients[1] * lambertValue * intersectionPointColor.red;
            green_component = c->green + temp_SpotLight->pointLight.color.green * this->coEfficients[1] * lambertValue * intersectionPointColor.green;
            blue_component = c->blue + temp_SpotLight->pointLight.color.blue * this->coEfficients[1] * lambertValue * intersectionPointColor.blue;

            c->setColor(red_component, green_component, blue_component);

            Point rayR_direction = (normal.direction * ((rayL.direction * normal.direction) * 2)) - rayL.direction;
            Ray rayR(intersectionPoint, rayR_direction);
            Point V = (r.direction * -1);

            double phongValue = max((rayR.direction * V), 0.0);

            red_component = c->red + temp_SpotLight->pointLight.color.red * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.red;
            green_component = c->green + temp_SpotLight->pointLight.color.green * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.green;
            blue_component = c->blue + temp_SpotLight->pointLight.color.blue * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.blue;

            c->setColor(red_component, green_component, blue_component);
        }

        // reflection part

        if (level >= recursion)
        {
            return tmin;
        }

        Point rayReflection_direction = (r.direction - (normal.direction * ((r.direction * normal.direction) * 2)));
        Ray rayReflect(intersectionPoint, rayReflection_direction);

        rayReflect.start = (rayReflect.start + (rayReflect.direction * epsilon));

        // find tmin from the nearest intersecting object,
        nearest = INT_MAX;
        tMinRef = INT_MAX;

        for (int i = 0; i < objects.size(); i++)
        {
            Color dummyColor;
            Object *tempObj = objects[i];
            tRef = tempObj->intersect(rayReflect, &dummyColor, 0);

            bool flag = tRef > 0 && tRef < tMinRef;

            if (flag)
            {
                tMinRef = tRef;
                nearest = i;
            }
        }

        Color *reflectColor = new Color;
        double dummyT;
        if (nearest != INT_MAX)
        {
            Object *tempMin = objects[nearest];
            dummyT = tempMin->intersect(rayReflect, reflectColor, level + 1);

            double red_component, green_component, blue_component;

            red_component = c->red + reflectColor->red * this->coEfficients[3];
            green_component = c->green + reflectColor->green * this->coEfficients[3];
            blue_component = c->blue + reflectColor->blue * this->coEfficients[3];

            c->setColor(red_component, green_component, blue_component);
        }

        return tmin;
    }
};

class General : public Object
{

public:
    double A, B, C, D, E, F, G, H, I, J;
    double quadA, quadB, quadC;
    General(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j)
    {
        this->A = a;
        this->B = b;
        this->C = c;
        this->D = d;
        this->E = e;
        this->F = f;
        this->G = g;
        this->H = h;
        this->I = i;
        this->J = j;
    }

    bool insideCube(Point p)
    {
        if (length != 0){
            if (!(p.x > referencePoint.x && p.x < (referencePoint.x + length))){
                return false;
            }
        }
        if(width != 0){
            if (!(p.y > referencePoint.y && p.y < (referencePoint.y + width))){
                return false;
            }
        }
        if(height != 0){
            if (!(p.z > referencePoint.z && p.z < (referencePoint.z + height))){
                return false;
            }
        }
        return true;
    }

    void quadValCalc(Ray r)
    {

        quadA = A * r.direction.x * r.direction.x + B * r.direction.y * r.direction.y + C * r.direction.z * r.direction.z + D * (r.direction.x) * (r.direction.y) + E * (r.direction.x) * (r.direction.z) + F * (r.direction.y) * (r.direction.z);

        quadB = 2 * A * (r.start.x) * (r.direction.x) + 2 * B * (r.start.y) * (r.direction.y) + 2 * C * (r.start.z) * (r.direction.z) + D * (((r.start.x) * (r.direction.y)) + ((r.start.y) * (r.direction.x))) + E * (((r.start.x) * (r.direction.z)) + ((r.start.z) * (r.direction.x))) + F * (((r.start.y) * (r.direction.z)) + ((r.start.z) * (r.direction.y))) + G * (r.direction.x) + H * (r.direction.y) + I * (r.direction.z);

        quadC = A * r.start.x * r.start.x + B * r.start.y * r.start.y + C * r.start.z * r.start.z + D * (r.start.x) * (r.start.y) + E * (r.start.x) * (r.start.z) + F * (r.start.y) * (r.start.z) + G * (r.start.x) + H * (r.start.y) + I * (r.start.z) + J;
    }

    double intersect(Ray r, Color *c, int level)
    {

        quadValCalc(r);

        double root_minus, root_plus, tmin;

        if (quadA != 0)
        {
            double diff = pow(quadB, 2) - 4 * quadA * quadC;
            if (diff > 0){
                root_minus = (-quadB - sqrt(diff)) / (2 * quadA);
                root_plus = (-quadB + sqrt(diff)) / (2 * quadA);
                if (root_minus > 0)
                {
                    Point p0 = (r.start + (r.direction * root_minus));
                    if (!insideCube(p0)){
                        root_minus = -1;
                    }
                }
                if (root_plus > 0)
                {
                    Point p1 = (r.start + (r.direction * root_plus));
                    if (!insideCube(p1)){
                        root_plus = -1;
                    }
                }

                if (root_minus > 0 && root_plus > 0)
                {   
                    tmin = min(root_minus, root_plus);
                }
                else if (root_minus > 0){
                    tmin = root_minus;
                }
                else if (root_plus > 0){
                    tmin = root_plus;
                }
                else{
                     return -1;  
                }
            }
            else
            {
                return -1;
            }
            
        }
        else
        {
            tmin = (-1) * (quadC / quadB);
        }

        if (level == 0)
        {
            return tmin;
        }

        // calculate intersection point
        Point intersectionPoint = (r.start + (r.direction * tmin));
        Color intersectionPointColor;

        intersectionPointColor.setColor(this->color.red, this->color.green, this->color.blue);

        // color = intersectionPointColor*coEfficient[AMB]
        double red_component = intersectionPointColor.red * this->coEfficients[0];
        double green_component = intersectionPointColor.green * this->coEfficients[0];
        double blue_component = intersectionPointColor.blue * this->coEfficients[0];

        c->setColor(red_component, green_component, blue_component);

        // calculate normal
        //Point normalDir;
        double x_value = 2 * A * intersectionPoint.x + D * intersectionPoint.y + E * intersectionPoint.z + G;
        double y_value = 2 * B * intersectionPoint.y + D * intersectionPoint.x + F * intersectionPoint.z + H;
        double z_value = 2 * C * intersectionPoint.z + E * intersectionPoint.x + F * intersectionPoint.y + I;
       
        Point normalDir(x_value, y_value, z_value);

        Ray normal(intersectionPoint, normalDir);

        double tempT, tempTmin, tRef, tMinRef;
        
        int nearest;

        for (int i = 0; i < pointLights.size(); i++)
        {
            PointLight *temp_PointLight = pointLights[i];
            Ray rayL(temp_PointLight->light_position, temp_PointLight->light_position - intersectionPoint);

            tempTmin = INT_MAX;

            for (int j = 0; j < objects.size(); j++)
            {
                Color *dummyColor = new Color();
                Object *tempObj = objects[i];
                tempT = tempObj->intersect(rayL, dummyColor, 0);
                bool flag = (tempT > 0 && tempT < tempTmin) ? 1 : 0;
                if (flag)
                {
                    tempTmin = tempT;
                }
            }

            // calculate shadow point
            Point shadowPoint = rayL.start + (rayL.direction * tempTmin);

            double distIntersect, distShadow;

            distIntersect = rayL.start.distanceCalc(intersectionPoint);
            distShadow = rayL.start.distanceCalc(shadowPoint);
            bool flag = (distShadow < distIntersect) ? 1 : 0;
            if (flag)
            {
                continue;
            }

            // calculate coeff

            double lambertValue = max(0.0, rayL.direction * normal.direction);
            double red_component, green_component, blue_component;
            red_component = c->red + temp_PointLight->color.red * this->coEfficients[1] * lambertValue * intersectionPointColor.red;
            green_component = c->green + temp_PointLight->color.green * this->coEfficients[1] * lambertValue * intersectionPointColor.green;
            blue_component = c->blue + temp_PointLight->color.blue * this->coEfficients[1] * lambertValue * intersectionPointColor.blue;

            c->setColor(red_component, green_component, blue_component);

            Point rayR_direction = (normal.direction * ((rayL.direction * normal.direction) * 2)) - rayL.direction;
            Ray rayR(intersectionPoint, rayR_direction);

            Point V = r.direction * -1;

            double phongValue = max((rayR.direction * V), 0.0);

            red_component = c->red + temp_PointLight->color.red * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.red;
            green_component = c->green + temp_PointLight->color.green * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.green;
            blue_component = c->blue + temp_PointLight->color.blue * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.blue;

            c->setColor(red_component, green_component, blue_component);
        }

        // spotlight
        for (int i = 0; i < spotLights.size(); i++)
        {

            SpotLight *temp_SpotLight = spotLights[i];
            Ray rayL(temp_SpotLight->pointLight.light_position, (temp_SpotLight->pointLight.light_position - intersectionPoint));
            Point SpotLight_direction = temp_SpotLight->lightDirection;
            SpotLight_direction.normalize();

            double cosVal = (SpotLight_direction * (rayL.direction * -1));
            double angle = (acos(cosVal) * 180) / pi;

            bool flag = angle > temp_SpotLight->cutOffAngle ? 1 : 0;

            if (flag)
            {
                continue;
            }

            tempTmin = INT_MAX;

            for (int j = 0; j < objects.size(); j++)
            {
                Color dummyColor;
                Object *tempObj = objects[i];
                tempT = tempObj->intersect(rayL, &dummyColor, 0);
                bool flag = (tempT > 0 && tempT < tempTmin) ? 1 : 0;
                if (flag)
                {
                    tempTmin = tempT;
                }
            }

            // calculate shadow point
            Point shadowPoint = (rayL.start + (rayL.direction * tempTmin));

            double distIntersect, distShadow;

            distIntersect = rayL.start.distanceCalc(intersectionPoint);
            distShadow = rayL.start.distanceCalc(shadowPoint);
            flag = (distShadow < distIntersect) ? 1 : 0;
            if (flag)
            {
                continue;
            }

            // calculate coeff

            // color += pl.color*coEfficient[DIFF]*lambertValue*intersectionPointColor

            double lambertValue = max(0.0, (rayL.direction * normal.direction));

            double red_component, green_component, blue_component;

            red_component = c->red + temp_SpotLight->pointLight.color.red * this->coEfficients[1] * lambertValue * intersectionPointColor.red;
            green_component = c->green + temp_SpotLight->pointLight.color.green * this->coEfficients[1] * lambertValue * intersectionPointColor.green;
            blue_component = c->blue + temp_SpotLight->pointLight.color.blue * this->coEfficients[1] * lambertValue * intersectionPointColor.blue;

            c->setColor(red_component, green_component, blue_component);

            Point rayR_direction = (normal.direction * ((rayL.direction * normal.direction) * 2)) - rayL.direction;
            Ray rayR(intersectionPoint, rayR_direction);
            Point V = (r.direction * -1);

            double phongValue = max((rayR.direction * V), 0.0);

            red_component = c->red + temp_SpotLight->pointLight.color.red * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.red;
            green_component = c->green + temp_SpotLight->pointLight.color.green * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.green;
            blue_component = c->blue + temp_SpotLight->pointLight.color.blue * this->coEfficients[2] * pow(phongValue, shininess) * intersectionPointColor.blue;

            c->setColor(red_component, green_component, blue_component);
        }

        // reflection part

        if (level >= recursion)
        {
            return tmin;
        }

        Point rayReflection_direction = (r.direction - (normal.direction * ((r.direction * normal.direction) * 2)));
        Ray rayReflect(intersectionPoint, rayReflection_direction);

        rayReflect.start = (rayReflect.start + (rayReflect.direction * epsilon));

        // find tmin from the nearest intersecting object,
        nearest = INT_MAX;
        tMinRef = INT_MAX;

        for (int i = 0; i < objects.size(); i++)
        {
            Color dummyColor;
            Object *tempObj = objects[i];
            tRef = tempObj->intersect(rayReflect, &dummyColor, 0);

            bool flag = tRef > 0 && tRef < tMinRef;

            if (flag)
            {
                tMinRef = tRef;
                nearest = i;
            }
        }

        Color *reflectColor = new Color;
        double dummyT;
        if (nearest != INT_MAX)
        {
            Object *tempMin = objects[nearest];
            dummyT = tempMin->intersect(rayReflect, reflectColor, level + 1);

            double red_component, green_component, blue_component;

            red_component = c->red + reflectColor->red * this->coEfficients[3];
            green_component = c->green + reflectColor->green * this->coEfficients[3];
            blue_component = c->blue + reflectColor->blue * this->coEfficients[3];

            c->setColor(red_component, green_component, blue_component);
        }

        return tmin;
    }
};
