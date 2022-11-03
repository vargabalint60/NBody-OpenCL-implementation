#include <map>
#include <utility>


double v = 1;
double camera_x = 1, camera_y = 2, camera_z = -5;
double camera_vx = 0, camera_vy  = 0, camera_vz = 0;
double phi = 0, theta = 2;
double phi_v = 0, theta_v = 0;
std::map<char, int> keys{{'w', 0}, {'a', 0}, {'s', 0}, {'d', 0}, 
                         {'u', 0}, {'l', 0}, {'o', 0}, {'r', 0}}; 

                        //u,l,o,r represents up, left, down, right keys

void control() {
    camera_vx = 0;
    camera_vy = 0;
    camera_vz = 0;
    if (keys['w'] == 1 && keys['a'] == 1 || keys['w'] == 1 && keys['d'] ||
        keys['s'] == 1 && keys['a'] == 1 || keys['s'] == 1 && keys['d']) {
        v = 1/sqrt(2);}
    else {v = 1;}

    if (keys['u'] == 1) {
        phi_v = 1;
    }
    if (keys['o'] == 1) {
        phi_v = -1;
    }
    if (keys['l'] == 1) {
        theta_v = -1;
    }
    if (keys['r'] == 1) {
        theta_v = 1;
    }
    phi += phi_v/200;
    theta += theta_v/200;

    if (keys['w'] == 1) {
        camera_vz += sin(theta)*cos(phi);
        camera_vy += sin(phi);
        camera_vx += cos(theta)*cos(phi);
    }
    if (keys['s'] == 1) {
        camera_vz += -sin(theta)*cos(phi);
        camera_vy += -sin(phi);
        camera_vx += -cos(theta)*cos(phi);
    }
    if (keys['a'] == 1) {
        camera_vz += -cos(theta)*cos(phi);
        camera_vx += sin(theta)*cos(phi);
    }
    if (keys['d'] == 1) {
        camera_vz += cos(theta)*cos(phi);
        camera_vx += -sin(theta)*cos(phi);
    }
    if (keys['e'] == 1) {
        camera_vy += 1;
    }
    if (keys['q'] == 1) {
        camera_vy -= 1;
    }
    camera_x += v*camera_vx/15;
    camera_y += v*camera_vy/15;
    camera_z += v*camera_vz/15;
    
}  

void reshape(int w, int h)
    {
        control();
        
        glViewport(0, 0, (GLsizei)w, (GLsizei)h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 2, 150.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(camera_x, camera_y, camera_z, camera_x + 3*cos(phi)*cos(theta),
                 camera_y + 3*sin(phi), camera_z + 3*sin(theta)*cos(phi), 0, 1, 0);
        glBegin(GL_POINTS);
        glColor3f(1, 1, 0);
        glVertex3f(camera_x + 3*cos(phi)*cos(theta), camera_y + 3*sin(phi), camera_z + 3*sin(theta)*cos(phi));
        glEnd();
        glFlush();
    }
 
 void Init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadIdentity();
    glOrtho(-200, 0.0, -50, 100, 70, 300);
}

void keyStroke(unsigned char c, int x, int y) {
    switch (c)
    {
        case 'w':
            keys['w'] = 1;
            break;  
        case 's':
            keys['s'] = 1;
            break;
        case 'e':
            keys['e'] = 1;
            break;  
        case 'q':
            keys['q'] = 1; 
            break;
        case 'a':
            keys['a'] = 1;
            break;  
        case 'd':
            keys['d'] = 1;
            break;
    }
}

void keyUp(unsigned char c, int x, int y) {
    switch (c)
    {
        case 'w':
            keys['w'] = 0;
            break;
        case 's':
            keys['s'] = 0;
            break;
        case 'e':
            keys['e'] = 0;
            break;  
        case 'q':
            keys['q'] = 0; 
            break;
        case 'a':
            keys['a'] = 0;
            break;  
        case 'd':
            keys['d'] = 0;
            break;   
 
    }
}

void specialKeyStroke(int c, int x, int y) {
    switch (c)
    {
        case GLUT_KEY_UP:
            phi_v = 1;
            break;
        case GLUT_KEY_DOWN:
            phi_v = -1;
            break;
        case GLUT_KEY_RIGHT:
            theta_v = 1;
            break;
        case GLUT_KEY_LEFT:
            theta_v = -1;
            break;    
    }
}

void specialKeyUp(int c, int x, int y) {
    switch (c)
    {
        case GLUT_KEY_UP:
            phi_v = 0;
            break;
        case GLUT_KEY_DOWN:
            phi_v = 0;
            break;
        case GLUT_KEY_RIGHT:
            theta_v = 0;
            break;
        case GLUT_KEY_LEFT:
            theta_v = 0;
            break;    
    }
}