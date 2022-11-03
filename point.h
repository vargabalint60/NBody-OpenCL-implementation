#pragma once

#include <GL/glut.h>
#include <CL/cl.h>
#include <iostream>
#include <math.h>

struct Point {
    double *x, *y, *z;
    double *vx, *vy, *vz;
    double *ax, *ay, *az;
    double *m;

    Point(int N) {
        x = new double[N];
        y = new double[N];
        z = new double[N];
        vx = new double[N];
        vy = new double[N];
        vz = new double[N];
        ax = new double[N];
        ay = new double[N];
        az = new double[N];
        m = new double[N];
        for (int i = 0; i < N; i++) {
            x[i] = 0.0;
            y[i] = 0.0;
            z[i] = 0.0;
            vx[i] = 0.0;
            vy[i] = 0.0;
            vz[i] = 0.0;
            ax[i] = 0.0;
            ay[i] = 0.0;
            az[i] = 0.0;
            m[i] = 1.0;
        }
    }
};

void initCircle(Point points, int N,int x0,int z0,int r) {
    for (int i = 0; i < N; i++) {
        points.x[i] = x0 + r*cos(2*M_PI/N*i);
        points.z[i] = z0 + r*sin(2*M_PI/N*i);
        points.y[i] = 0;
    }
}

void initPoints(Point points) {
    points.x[0] = 0.0;
    points.y[0] = 3.0;
    points.z[0] = 5.0;
    points.vx[0] = 1;
    points.m[0] = 5;

    points.x[1] = 0.0;
    points.y[1] = 0.0;
    points.z[1] = 8.0;
    points.vx[1] = -0.6;
    points.m[1] = 10;

    points.x[2] = -1.0;
    points.y[2] = -1.0;
    points.z[2] = 3.0;
    points.vx[2] = 0.1;
    points.m[2] = 20;

    points.x[3] = 4.0;
    points.y[3] = 3.0;
    points.z[3] = 5.0;
    points.vx[3] = 1;
    points.m[3] = 2;

    points.x[4] = 5.0;
    points.y[4] = 1.0;
    points.z[4] = 5.0;
    points.vx[4] = 0.5;
    points.m[4] = 5;

    points.x[5] = 0.0;
    points.y[5] = 0.0;
    points.z[5] = 0.0;
    points.vx[5] = 1;
    points.m[5] = 10;

    points.x[6] = -1.0;
    points.y[6] = -4.0;
    points.z[6] = -2.0;
    points.vx[6] = 0;
    points.m[6] = 8;

    points.x[7] = -3.0;
    points.y[7] = -5.0;
    points.z[7] = -2.0;
    points.vy[7] = -1;
    points.m[7] = 5;
    //points[0] = Point(-5.0, 3.0, 5.0);
    //points[1] = Point(-5.0, 0.0, 10.0);
}


void initSolar(Point points) {
    points.x[0] = 0.0;
    points.y[0] = 0.0;
    points.z[0] = 0.0;
    points.m[0] = 1000;

    points.x[1] = 5.0;
    points.y[1] = 0.0;
    points.z[1] = 0.0;
    points.vz[1] = sqrt(abs(points.m[0]/points.x[1]));
    points.m[1] = 2;

    points.x[2] = -8.0;
    points.y[2] = 0.0;
    points.z[2] = 0.0;
    points.vz[2] = -sqrt(abs(points.m[0]/points.x[2]));
    points.m[2] = 8;

    points.x[9] = points.x[2] + 0.5;
    points.y[9] = 0.0;
    points.z[9] = 0.0;
    points.vz[9] = -sqrt(abs(points.m[0]/points.x[2])) + 
                    sqrt(abs(points.m[2]/(points.x[9] - points.x[2])));
    points.m[9] = 0.2;

    points.x[3] = 0.0;
    points.y[3] = 0.0;
    points.z[3] = 3.0;
    points.vx[3] = -sqrt(abs(points.m[0]/points.z[3]));
    points.m[3] = 1;

    points.x[10] = 0.0;
    points.y[10] = 0.0;
    points.z[10] = 1.3;
    points.vx[10] = -sqrt(abs(points.m[0]/points.z[10]));
    points.m[10] = 0.8;

    points.x[4] = 0.0;
    points.y[4] = 0.0;
    points.z[4] = -18.0;
    points.vx[4] = sqrt(abs(points.m[0]/points.z[4]));
    points.m[4] = 20;

    points.x[8] = 0.0;
    points.y[8] = 0.0;
    points.z[8] = points.z[4] + 0.4;
    points.vx[8] = -sqrt(abs(points.m[0]/points.z[4])) + 
                    sqrt(abs(points.m[4]/(points.z[8] - points.z[4])));
    points.m[8] = 0.6;

    points.x[5] = 0.0;
    points.y[5] = 0.0;
    points.z[5] = 27.0;
    points.vx[5] = -sqrt(abs(points.m[0]/points.z[5]));
    points.m[5] = 28;

    points.x[6] = 0.0;
    points.y[6] = 0.0;
    points.z[6] = points.z[5] + 0.5;
    points.vx[6] = -sqrt(abs(points.m[0]/points.z[5])) - 
                    sqrt(abs(points.m[5]/(points.z[6] - points.z[5])));
    points.m[6] = 0.5;

    points.x[7] = 0.0;
    points.y[7] = 0.0;
    points.z[7] = points.z[5] + 1;
    points.vx[7] = -sqrt(abs(points.m[0]/points.z[5])) -
                    sqrt(abs(points.m[5]/(points.z[7] - points.z[5])));
    points.m[7] = 0.3;

    /*points[11].x = 100;
    points[11].y = 10.0;
    points[11].z = 10.0;
    points[11].vx = -10;
    points[11].m = 1000;*/

    double alpha = 0.6;

    for (int i = 11; i < 22; i++) {
        points.m[i] = points.m[i-11];
        points.x[i] = points.x[i-11]*cos(alpha) + 50;
        points.z[i] = points.z[i-11] + 23;
        points.y[i] = points.z[i-11]*sin(alpha);
        points.vx[i] = points.vx[i-11]*cos(alpha) - 1.9;
        points.vz[i] = points.vz[i-11] + 0.1;
        points.vy[i] = points.vz[i-11]*sin(alpha) + 0.1;
    }

    for (int i = 0; i < 11; i++) {
        points.vx[i] = points.vx[i] + 1.9;
        points.x[i] = points.x[i] - 50;
        points.vz[i] = points.vz[i] - 0.31;
        points.z[i] = points.z[i] - 23;

    }
}
