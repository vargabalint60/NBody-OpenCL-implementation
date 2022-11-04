#include<stdio.h>
#include<GL/glut.h>
#include<math.h>
#include <CL/cl.h>
#include <iostream>
#include <vector>
#include "point.h"
#include "glInit.h"
#include "create_context.h"
#define _USE_MATH_DEFINES

const int N = 100;
Point points(N);
int x0 = 0, z0 = 5, r = 2;
double dt = 0.005;
double G = 1;

int w = 1366, h = 768;

int* buffer = new int[w*h];

//const char* cmd = "ffmpeg -r 120 -f rawvideo -pix_fmt rgba -s 1366x768 -i - "
//                  "-threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip output.mp4";
//FILE* ffmpeg = popen(cmd, "w");

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < N; i++) {
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
    calcAcc(points, N);
    calcVelPos(points, N);
    glutPostRedisplay();
    glutTimerFunc(5, timer, value);
}


int main (int argc, char** argv)
{
    create_context();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glEnable(GL_COLOR_MATERIAL);
      
    // giving window size in X- and Y- direction
    glutInitWindowSize(w, h);
    glutInitWindowPosition(0, 0);
      
    // Giving name to window
    glutCreateWindow("N Body Simulation");
    Init();
    reshape(w, h);

    initCircle(points, N, x0, z0, r);
    //initSolar(points);
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