#include<bits/stdc++.h>
using namespace std;
#include "matrix.h"
#include "bitmap_image.hpp"

void init_stack(stack<Matrix> &matrix_stack){
    Matrix I;
    I.generateIdentity();
    matrix_stack.push(I);
}

int main(int argc, char* argv[]){
    /*
        stage 1 => Modeling Transformation
    */
    string test_no = argv[1];

    ifstream input("./" + test_no + "/scene.txt");
    ofstream output("./" + test_no + "/stage1.txt");
    
    double eye_x, eye_y, eye_z;
    double look_x, look_y, look_z;
    double up_x, up_y, up_z;
    double fovY, aspectRatio, near, far;
    
    input >> eye_x >> eye_y >> eye_z;
    input >> look_x >> look_y >> look_z;
    input >> up_x >> up_y >> up_z;
    input >> fovY >> aspectRatio >> near >> far;

    Point eye(eye_x, eye_y, eye_z);
    Point look(look_x, look_y, look_z);
    Point up(up_x, up_y, up_z);

    stack<Matrix> matrix_stack;
    
    init_stack(matrix_stack);

    string command;

    while(true){
        input >> command;

        if(command.compare("triangle") == 0){
            Point p1, p2, p3;
            input >> p1 >> p2 >> p3;
            Triangle t(p1, p2, p3);
            t = matrix_stack.top()*t;
            output << t.a << t.b << t.c << endl;
        }
        else if(command.compare("translate") == 0){
            Point p;
            input >> p;
            Matrix matrix;
            matrix.translate(p.x, p.y, p.z);
            matrix_stack.top() = matrix_stack.top().multiply_matrix(matrix);
        }
        else if(command.compare("scale") == 0){
            Point p;
            input >> p;
            Matrix matrix;
            matrix.scale(p.x, p.y, p.z);
            matrix_stack.top() = matrix_stack.top().multiply_matrix(matrix);
        }
        else if(command.compare("rotate") == 0){
            Point p;
            double theta;
            input >> theta >> p;
            Matrix matrix;
            matrix.rotate(p, theta*PI/180.0);
            matrix_stack.top() = matrix_stack.top().multiply_matrix(matrix);
        }
        else if(command.compare("push") == 0){
            matrix_stack.push(matrix_stack.top());
        }
        else if(command.compare("pop") == 0){
            matrix_stack.pop();
        }
        else if(command.compare("end") == 0){
            break;
        } 
    }
   
    /*
        stage 2 => View Transformation
    */
    input.close();
    output.close();
    input.open("./" + test_no + "/stage1.txt");
    output.open("./" + test_no + "/stage2.txt");

    Matrix V = generateViewMatrix(eye, look, up);

    Triangle triangle;

    while(true){
        Point p1, p2, p3;
        input >> p1 >> p2 >> p3;
        triangle = Triangle(p1, p2, p3);
        if(input.eof()) {
            break;
        }
        triangle = V*triangle;
        output << triangle.a << triangle.b << triangle.c << endl;
    }

    input.close();
    output.close();

    /*
        stage 3 => Projection Transformation
    */

    input.open("./" + test_no + "/stage2.txt");
    output.open("./" + test_no + "/stage3.txt");

    Matrix P = generateProjectionMatrix(fovY, aspectRatio, near, far); 

   
    while(true){
        Point p1, p2, p3;
        input >> p1 >> p2 >> p3;
        triangle = Triangle(p1, p2, p3);
        if(input.eof()) {
            break;
        }
        triangle = P*triangle;
        output << triangle.a << triangle.b << triangle.c << endl;
    }

    input.close();
    output.close();

    /*
        stage 4: Clipping and Scan Conversion using Z-Buffer
    */

    input.open("./" + test_no + "/stage3.txt");
    output.open("./" + test_no + "/z_buffer.txt");

    ifstream config("./" + test_no + "/config.txt");
    int screen_width, screen_height;
    config >> screen_width >> screen_height;
    
    double box_left = -1, box_right = 1; 
    double box_bottom = -1, box_top = 1;
    double Z_min = -1, Z_max = 1;

    double dx = (box_right-box_left)/screen_width;
    double dy = (box_top-box_bottom)/screen_height;

    double Top_Y = box_top - dy/2.0;
    double Left_X = box_left + dx/2.0;

    bitmap_image image(screen_width, screen_height);
    image.set_all_channels(0, 0, 0);

    // initialize z-buffer and frame buffer

    vector< vector<double> > z_buffer(screen_height, vector<double>(screen_width, Z_max));

    int i = 0;
    while(i < screen_height){
        int j = 0;
        while(j < screen_width){
            z_buffer[i][j] = Z_max;
            j++;
        }
        i++;
    }

    while(true){
        Point p1, p2, p3;
        input >> p1 >> p2 >> p3;
        triangle = Triangle(p1, p2, p3);
        if(input.eof()) {
            break;
        }

        triangle.sort();
        triangle.recolor();
        
        // clipping
        double min_y = triangle.a.y;
        double max_y = triangle.b.y;
       
        min_y = max(min_y, box_bottom);
        max_y = min(max_y, box_top);

        // scan conversion
        double y = min_y;

        while(y <= max_y){
           
            // compute min_x and max_x for this row
            double min_x, max_x;
            double min_z, max_z;

            min_x = min_z = 0;
            max_x = max_z = -1;


            if(!((triangle.b.y == triangle.a.y) || (triangle.a.y == triangle.c.y))){
                double value1 = (triangle.b.x-triangle.a.x)*(y-triangle.a.y);
                double value2 = (triangle.b.y-triangle.a.y);
                min_x = triangle.a.x + (value1/value2);

                value1 = (triangle.c.x-triangle.a.x)*(y-triangle.a.y);
                value2 = (triangle.c.y-triangle.a.y);
                max_x = triangle.a.x + (value1/value2);

                value1 = (triangle.b.z-triangle.a.z)*(y-triangle.a.y);
                value2 = (triangle.b.y-triangle.a.y);
                min_z = triangle.a.z + (value1/value2);
                
                value1 = (triangle.c.z-triangle.a.z)*(y-triangle.a.y);
                value2 = (triangle.c.y-triangle.a.y);
                max_z = triangle.a.z + (value1/value2);

                if( min_x < max_x ) {
                    continue;
                }
                else{
                    swap(min_x, max_x);
                    swap(min_z, max_z);
                }
            }

            min_x = max(min_x, box_left);
            max_x = min(max_x, box_right);

            double x = min_x;

            while(x <= max_x){
                if( max_x != min_x ){
                    int i = (Top_Y-y)/dy;
                    int j = (x-Left_X)/dx;
                
                    double z = min_z + (max_z-min_z)*(x-min_x)/(max_x-min_x);
                    if(!((z > z_buffer[i][j]) || (z < Z_min))){
                        z_buffer[i][j] = z;
                        image.set_pixel(j, i, triangle.color.r, triangle.color.g , triangle.color.b);
                    }
                } 
                else {
                    continue;
                }
                x += dx;
            }
            y += dy;
        }


        min_y = triangle.b.y;
        max_y = triangle.c.y;
       
        min_y = max(min_y, box_bottom);
        max_y = min(max_y, box_top);


        // scan conversion
        y = min_y;

        while(y <= max_y){
            // compute min_x and max_x for this row
            double min_x, max_x;
            double min_z, max_z;

            min_x = min_z = 0;
            max_x = max_z = -1;

            if(!((triangle.b.y == triangle.c.y) || (triangle.a.y == triangle.c.y))){
                double value1 = (triangle.b.x-triangle.c.x)*(y-triangle.c.y);
                double value2 = (triangle.b.y-triangle.c.y);
                min_x = triangle.c.x + (value1/value2);
                
                value1 = (triangle.c.x-triangle.a.x)*(y-triangle.a.y);
                value2 = (triangle.c.y-triangle.a.y);
                max_x = triangle.a.x + (value1/value2);
               

                value1 = (triangle.b.z-triangle.c.z)*(y-triangle.c.y);
                value2 = (triangle.b.y-triangle.c.y);
                min_z = triangle.c.z + (value1/value2);
                
                value1 = (triangle.c.z-triangle.a.z)*(y-triangle.a.y);
                value2 = (triangle.c.y-triangle.a.y);
                max_z = triangle.a.z + (value1/value2);

                if( min_x > max_x ) {
                    swap(min_x, max_x);
                    swap(min_z, max_z);
                }
            }

            min_x = max(min_x, box_left);
            max_x = min(max_x, box_right);

            double x = min_x;

            while(x<=max_x){
                if( max_x != min_x ){
                    int i = (Top_Y-y)/dy;
                    int j = (x-Left_X)/dx;
                
                    double z = min_z + (max_z-min_z)*(x-min_x)/(max_x-min_x);
                    if(!((z > z_buffer[i][j]) || (z < Z_min))){
                        z_buffer[i][j] = z;
                        image.set_pixel(j, i, triangle.color.r, triangle.color.g , triangle.color.b);
                    }
                } else{
                    continue;
                } 
                x += dx;
            }
            y += dy;
        }

    }

    image.save_image("./" + test_no + "/out.bmp");

    i = 0;
    while(i < screen_height){
        int j = 0;
        while(j < screen_width){
            if(z_buffer[i][j] < Z_max){
                output << setprecision(6) << fixed << z_buffer[i][j] << "\t";
            }
            j++;
        }
        output << endl;
        i++;
    }

    input.close();
    config.close();
    output.close();
    
    z_buffer.clear();
    z_buffer.shrink_to_fit();
    
    return 0;
}
