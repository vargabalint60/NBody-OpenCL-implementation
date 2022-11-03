#include<stdio.h>
#include<GL/glut.h>
#include<math.h>
#include <CL/cl.h>
#include <iostream>
#include <vector>
#include "point.h"
#include "glInit.h"
#define _USE_MATH_DEFINES

const int N = 22;
Point points(N);
int x0 = 0, z0 = 5, r = 2;
double dt = 0.005;
double G = 1;

int w = 1366, h = 768;

int* buffer = new int[w*h];

//const char* cmd = "ffmpeg -r 120 -f rawvideo -pix_fmt rgba -s 1366x768 -i - "
//                  "-threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip output.mp4";
//FILE* ffmpeg = popen(cmd, "w");

void deriv(Point points, int i, int j) {
    double r_x = points.x[i] - points.x[j]; 
    double r_y = points.y[i] - points.y[j];
    double r_z = points.z[i] - points.z[j];
    double rSquared = r_x*r_x + r_y*r_y + r_z*r_z;
    double rCubed = rSquared * sqrt(rSquared);

    if (rSquared > 0.1) {
    points.ax[i] += - G*points.m[j] * r_x / rCubed;
    points.ay[i] += - G*points.m[j] * r_y / rCubed;
    points.az[i] += - G*points.m[j] * r_z / rCubed;
    }
}

void acc(int i) {
    for (int j = 0; j < N; j++) {
        if (j != i) {
             deriv(points, i, j); 
        }
    }
}

void move(int i) {
    points.vx[i] += points.ax[i]*dt;
    points.vy[i] += points.ay[i]*dt;
    points.vz[i] += points.az[i]*dt;
    points.x[i] += points.vx[i]*dt;
    points.y[i] += points.vy[i]*dt;
    points.z[i] += points.vz[i]*dt; 
}


void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < N; i++) {
        /*glPointSize(sqrt(point.m));
        glBegin(GL_POINTS);
        glColor3f(1, 1, 1);
        //td::cout << point.m << "\n";
        
        glVertex3f(point.x, point.y, point.z); 
        glEnd();  */

        glColor3f(1, 1, 1);
        glPushMatrix();
        glTranslatef(points.x[i], points.y[i], points.z[i]);
        glutSolidSphere(sqrt(points.m[i])/40 + 0.05, 30, 30);
        glPopMatrix();
        
    }

    glBegin(GL_LINES);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 10, 0);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(10, 0, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 10);
    glEnd();
    glFlush();

    //std::cout << camera_vx << "\n";  
    reshape(w, h);
    //glutSwapBuffers();
    //glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    //fwrite(buffer, sizeof(int)*w*h, 1, ffmpeg);
    
}

void timer(int value) {
    for (int i = 0; i < N; i++) {
        points.ax[i] = 0;
        points.ay[i] = 0;
        points.az[i] = 0;
        acc(i);
    }
    for (int i = 0; i < N; i++) {
        move(i);
    }
    glutPostRedisplay();
    glutTimerFunc(5, timer, value);
}

/*void timer(int value) {
    for (int i = 0; i < N; i++) {
        points.M[i]ove();
    }
    glutPostRedisplay();
    glutTimerFunc(5, timer, value);
}*/

int main (int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glEnable(GL_COLOR_MATERIAL);
      
    // giving window size in X- and Y- direction
    glutInitWindowSize(w, h);
    glutInitWindowPosition(0, 0);
      
    // Giving name to window
    glutCreateWindow("Circle Drawing");
    Init();
    reshape(w, h);

    //initCircle(points, N, x0, z0, r);
    initSolar(points);
    //initPoints(points);
    glutKeyboardFunc(keyStroke);
    glutSpecialFunc(specialKeyStroke);
    glutKeyboardUpFunc(keyUp);
    glutSpecialUpFunc(specialKeyUp);
    glutDisplayFunc(display);
    glutTimerFunc(5, timer, 0);
    glutMainLoop();
    //pclose(ffmpeg);
}