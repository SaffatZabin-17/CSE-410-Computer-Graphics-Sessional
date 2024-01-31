#include<bits/stdc++.h>
#include "triangle.cpp"
using namespace std;

#define PI 2.0*acos(0.0)

/*
A Matrix structure
*/

class Matrix{

    Point RodriguesFormula(Point x, Point a, double theta){
        a.normalize();
        return x*cos(theta)+a*(a*x)*(1-cos(theta))+(a^x)*sin(theta);
    }

public:

    double matrix[4][4];

    Matrix(){
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                matrix[i][j] = 0.0;
            }
        }
    }

    Matrix multiply_matrix(Matrix m){
        Matrix temp;
        int i = 0;
        int j, k;
        while(i < 4){
            j = 0;
            while(j < 4){
                k = 0;
                while(k < 4){
                    temp.matrix[i][j] += matrix[i][k] * m.matrix[k][j];
                    k++;
                }
                j++;
            }
            i++;
        }
        return temp;
    }

    Point multiply_point(Point point){
        double temp[4] = {0.0};

        for(int i=0; i<4; i++) {
            for(int j=0; j<4; j++) {
                if(j == 0){
                    temp[i] += matrix[i][j]*point.x;
                } else if(j == 1){
                    temp[i] += matrix[i][j]*point.y;
                } else if(j == 2){
                    temp[i] += matrix[i][j]*point.z;
                } else{
                    temp[i] += matrix[i][j]*point.n;
                }
            }
        }

        temp[0] /= temp[3];
        temp[1] /= temp[3];
        temp[2] /= temp[3];

        Point ret(temp[0], temp[1], temp[2], temp[3]);
        return ret;
    }

    Triangle operator*(Triangle t){

        Triangle ret;
        ret.a = (*this).multiply_point(t.a);
        ret.b = (*this).multiply_point(t.b);
        ret.c = (*this).multiply_point(t.c);
        return ret;
    }

    void generateIdentity(){
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                if( i == j ) {
                    matrix[i][j] = 1.0;
                }   else {
                    matrix[i][j] = 0.0;
                }
            }
        }
    }

    void translate(double tx, double ty, double tz){
        generateIdentity();
        matrix[0][3] = tx;
        matrix[1][3] = ty;
        matrix[2][3] = tz;
    }
    

    void scale(double sx, double sy, double sz){
        generateIdentity();
        matrix[0][0] = sx;
        matrix[1][1] = sy;
        matrix[2][2] = sz;
    }

    void rotate(Point a,double theta){
        generateIdentity();
       
        Point ax = RodriguesFormula(Point(1.0, 0.0, 0.0), a, theta);
        Point ay = RodriguesFormula(Point(0.0, 1.0, 0.0), a, theta);
        Point az = RodriguesFormula(Point(0.0, 0.0, 1.0), a, theta);

        matrix[0][0] = ax.x;
        matrix[1][0] = ax.y;
        matrix[2][0] = ax.z;

        matrix[0][1] = ay.x;
        matrix[1][1] = ay.y;
        matrix[2][1] = ay.z;

        matrix[0][2] = az.x;
        matrix[1][2] = az.y;
        matrix[2][2] = az.z;
    }

    friend ostream& operator<<(ostream& os, const Matrix& m){
        int i = 0, j = 0;
        while(i < 4){
            j = 0;
            while(j < 4){
                os << m.matrix[i][j] << " ";
                j++;
            }
            os << endl;
            i++;
        }
        return os;
    }
};

Matrix generateViewMatrix(Point eye, Point look, Point up){
    Point l,r,u;
    l = look - eye;
    l.normalize();
    r = l^up;
    r.normalize();
    u = r^l;

    Matrix T;
    T.translate(-eye.x, -eye.y, -eye.z);

    Matrix R;
    R.generateIdentity();

    R.matrix[0][0] = r.x; 
    R.matrix[0][1] = r.y; 
    R.matrix[0][2] = r.z;

    R.matrix[1][0] = u.x; 
    R.matrix[1][1] = u.y; 
    R.matrix[1][2] = u.z;

    R.matrix[2][0] = -l.x; 
    R.matrix[2][1] = -l.y; 
    R.matrix[2][2] = -l.z;

    Matrix V = R.multiply_matrix(T);
    return V;
}

Matrix generateProjectionMatrix(double fovY, double aspectRatio, double near, double far){
    Matrix P;
    P.generateIdentity();

    double fovX = fovY * aspectRatio;
    double t = near * tan((fovY/2.0)*(PI/180.0));
    double r = near * tan((fovX/2.0)*(PI/180.0));

    P.matrix[0][0] = near/r;
    P.matrix[1][1] = near/t;
    P.matrix[2][2] = -(far+near)/(far-near);
    P.matrix[2][3] = -(2.0*far*near)/(far-near);
    P.matrix[3][2] = -1.0;
    P.matrix[3][3] = 0.0;

    return P;
}
