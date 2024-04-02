#include <GL/glut.h>
#include <stdlib.h>
#include "scene.h"

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Rendering Scene 3D");
    init();
    loadTexture();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(25, update, 0); // Iniciando o temporizador
    glutMainLoop();
    return 0;
}
