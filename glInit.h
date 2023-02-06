#include <map>
#include <utility>


double v = 1;
double camera_x = 1, camera_y = 2, camera_z = -5;
double camera_vx = 0, camera_vy  = 0, camera_vz = 0;
double phi = 0, theta = 2;
double phi_v = 0, theta_v = 0;
std::map<char, bool> keys{{'w', false}, {'a', false}, {'s', false}, {'d', false}, 
                         {'u', false}, {'l', false}, {'o', false}, {'r', false}}; 

                        //u,l,o,r represents up, left, down, right keys

void control() {
    camera_vx = 0;
    camera_vy = 0;
    camera_vz = 0;
    phi_v = 0;
    theta_v = 0;
    if (keys['w'] && keys['a'] || keys['w'] && keys['d'] ||
        keys['s'] && keys['a'] || keys['s'] && keys['d']) {
        v = 1/sqrt(2);}
    else {v = 1;}

    if (keys['u'] && !keys['o']) {
        phi_v = 1;
    }
    if (keys['o'] && !keys['u']) {
        phi_v = -1;
    }
    if (keys['l'] && !keys['r']) {
        theta_v = -1;
    }
    if (keys['r'] && !keys['l']) {
        theta_v = 1;
    }
    phi += phi_v/200;
    theta += theta_v/200;

    if (keys['w']) {
        camera_vz += sin(theta)*cos(phi);
        camera_vy += sin(phi);
        camera_vx += cos(theta)*cos(phi);
    }
    if (keys['s']) {
        camera_vz += -sin(theta)*cos(phi);
        camera_vy += -sin(phi);
        camera_vx += -cos(theta)*cos(phi);
    }
    if (keys['a']) {
        camera_vz += -cos(theta)*cos(phi);
        camera_vx += sin(theta)*cos(phi);
    }
    if (keys['d']) {
        camera_vz += cos(theta)*cos(phi);
        camera_vx += -sin(theta)*cos(phi);
    }
    if (keys['e']) {
        camera_vz += -sin(theta)*sin(phi);
        camera_vy += cos(phi);
        camera_vx += -cos(theta)*sin(phi);
    }
    if (keys['q']) {
        camera_vz += sin(theta)*sin(phi);
        camera_vy += -cos(phi);
        camera_vx += cos(theta)*sin(phi);
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
            keys['w'] = true;
            break;  
        case 's':
            keys['s'] = true;
            break;
        case 'e':
            keys['e'] = true;
            break;  
        case 'q':
            keys['q'] = true; 
            break;
        case 'a':
            keys['a'] = true;
            break;  
        case 'd':
            keys['d'] = true;
            break;
    }
}

void keyUp(unsigned char c, int x, int y) {
    switch (c)
    {
        case 'w':
            keys['w'] = false;
            break;
        case 's':
            keys['s'] = false;
            break;
        case 'e':
            keys['e'] = false;
            break;  
        case 'q':
            keys['q'] = false; 
            break;
        case 'a':
            keys['a'] = false;
            break;  
        case 'd':
            keys['d'] = false;
            break;   
 
    }
}

void specialKeyStroke(int c, int x, int y) {
    switch (c)
    {
        case GLUT_KEY_UP:
            keys['u'] = true;
            break;
        case GLUT_KEY_DOWN:
            keys['o'] = true;
            break;
        case GLUT_KEY_RIGHT:
            keys['r'] = true;
            break;
        case GLUT_KEY_LEFT:
            keys['l'] = true;
            break;    
    }
}

void specialKeyUp(int c, int x, int y) {
    switch (c)
    {
        case GLUT_KEY_UP:
            keys['u'] = false;
            break;
        case GLUT_KEY_DOWN:
            keys['o'] = false;
            break;
        case GLUT_KEY_RIGHT:
            keys['r'] = false;
            break;
        case GLUT_KEY_LEFT:
            keys['l'] = false;
            break;    
    }
}