#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <functional>
#include <stack>
#include <algorithm>
#include <cmath>
#include "desenhos.h"
#include "perif.h"

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    currColorArray[0] = 0.0f;
    currColorArray[1] = 0.0f;
    currColorArray[2] = 0.0f;
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1280, 900);
    glutCreateWindow("Paint?");
    glutPositionWindow(100, 50);
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMover);
    glutKeyboardFunc(keyboard);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    std::vector<std::pair<float, float>> pontos;
    glutMainLoop();
    return 0;

}
