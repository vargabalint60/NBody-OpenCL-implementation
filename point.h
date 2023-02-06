#pragma once

#include <GL/glut.h>
#include <CL/cl.h>
#include <iostream>
#include <math.h>
#include <vector>

struct Point {
    double *x = new double[N]{0}, *y = new double[N]{0}, *z = new double[N]{0};
    double *vx = new double[N]{0}, *vy = new double[N]{0}, *vz = new double[N]{0};
    double *ax = new double[N]{0}, *ay = new double[N]{0}, *az = new double[N]{0};
    double *m = new double[N]{0};
    int *blockId = new int[N]{0};
    int *pairs = new int[N*(N-1)]{0};

};

void initCircle(Point points, int N,int x0,int z0,int r) {
    for (int i = 0; i < N; i++) {
        points.x[i] = x0 + r*cos(2*M_PI/N*i);
        points.z[i] = z0 + r*sin(2*M_PI/N*i);
        points.y[i] = 0;
    }
}


void initRand(Point points, int N) {  //initiate random coords
    int l = 50;
    for (int i = 0; i < N; i++) {
        points.x[i] = (double)(rand()%(l*1000))/1000-25;
        points.y[i] = (double)(rand()%(l*1000))/1000-25;
        points.z[i] = (double)(rand()%(l*1000))/1000-25;
        points.m[i] = rand()%50+1;
    }
}

void initSolar(Point points) {  //initiate two solar systems with 22 bodies
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

    double alpha = 0;

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


struct Block {  //datastructure for the containment of the points
    std::vector<double> xc, yc, zc;  //geometrical center of cube
    std::vector<double> xm, ym, zm;  //center of mass
    std::vector<double> m;
    std::vector<double> ax;
    std::vector<double> ay;
    std::vector<double> az;
    double l;

    Block(double l) : l(l) {}

    void calcCoords(Point points, int N) {
        empty();
        bool in_list;
        double bx, by, bz;
        for (int i = 0; i < N; i++) {
            in_list = false;
            bx = l/2 + l*calcInd(points.x[i], l);  //center of the cube
            by = l/2 + l*calcInd(points.y[i], l);  //that contains points[i]
            bz = l/2 + l*calcInd(points.z[i], l);

            for (int j = 0; j < xc.size(); j++) {
                if (bx == xc[j] && by == yc[j] && bz == zc[j]) {
                    in_list = true;
                    xm[j] += points.m[i]*points.x[i];
                    ym[j] += points.m[i]*points.y[i];
                    zm[j] += points.m[i]*points.z[i];
                    m[j] += points.m[i];
                    points.blockId[i] = j;
                    break;
                }
            }
            if (!in_list) {
                points.blockId[i] = xc.size();
                xc.push_back(bx);
                yc.push_back(by);
                zc.push_back(bz);
                xm.push_back(points.m[i]*points.x[i]);
                ym.push_back(points.m[i]*points.y[i]);
                zm.push_back(points.m[i]*points.z[i]);
                m.push_back(points.m[i]);
            }
        }

        for (int j = 0; j < xc.size(); j++) {
            xm[j] /= m[j];
            ym[j] /= m[j];
            zm[j] /= m[j];
        }


        for (int i = 0; i < xc.size(); i++) {
            ax.push_back(0);
            ay.push_back(0);
            az.push_back(0);
            for (int j = 0; j < xc.size(); j++) {
                if (i != j) {
                    double r_x = xm[i] - xm[j]; 
                    double r_y = ym[i] - ym[j];
                    double r_z = zm[i] - zm[j];

                    double rSquared = r_x*r_x + r_y*r_y + r_z*r_z;
                    double rCubed = rSquared * sqrt(rSquared);

                    if (rSquared > 0.2) {
                        ax[i] += - m[j] * r_x / rCubed;   //missing G
                        ay[i] += - m[j] * r_y / rCubed;
                        az[i] += - m[j] * r_z / rCubed;
                    }
                    else {
                        ax[i] += m[j] * r_x / (5*rCubed); 
                        ay[i] += m[j] * r_y / (5*rCubed);
                        az[i] += m[j] * r_z / (5*rCubed);
                    }
                }
            }
        }
    }

    void empty() {
        xc.clear();
        yc.clear();
        zc.clear();
        xm.clear();
        ym.clear();
        zm.clear();
        m.clear();
        ax.clear();
        ay.clear();
        az.clear();
    }

    template <typename T>
    int sgn(T x) {
        return (x < 0) ? -1 : 0;
    }

    int calcInd(double coord, double l) { 
        return (int)(coord/l) + sgn(coord); 
    }
};