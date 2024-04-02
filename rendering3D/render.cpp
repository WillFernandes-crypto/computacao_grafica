#include <GL/glut.h>
#include <stdlib.h>
#include "render.h"

// Variáveis para controle de animação
float teapotAngle = 0.0;
bool rotateTeapot = false; // Variável para controlar a rotação

void init(void) {
    // Configuração da luz
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 }; // Iluminação ambiente
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; // Iluminação difusa
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // Iluminação especular
    GLfloat light_position[] = { 2.0, 2.0, 2.0, 0.0 }; // Posição da luz

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glClearColor(0.5, 0.5, 0.5, 0.0);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
}

void teapot() {
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat high_shininess[] = { 100.0 };
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    glColor3f(0.4, 0.5, 0.2);
    glRotatef(teapotAngle, 0.0, 1.0, 0.0); // Rotação adicional para animação no eixo Y
    glutSolidTeapot(0.5);
    glPopMatrix();
}

void viewXY() {

    // Viewport 1: XY
    glViewport(0, 250, 400, 400);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.5, 1.5, -1.5, 1.5, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    teapot(); // Desenhe o teapot sem rotação adicional

}

void viewXZ() {

    // Viewport 2: XZ
    glViewport(250, 250, 400, 400);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.5, 1.5, -1.5, 1.5, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(90.0, 0.0, 1.0, 0.0); // Rotação para visualização XZ
    teapot();

}

void viewYZ() {

    // Viewport 3: YZ
    glViewport(0, 0, 400, 400);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.5, 1.5, -1.5, 1.5, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(90.0, 1.0, 0.0, 0.0); // Rotação para visualização YZ
    teapot();

}

void viewPerspectiva() {

    // Viewport 4: Perspective
    glViewport(250, 0, 400, 400);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0, 1.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.4, 0.7, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // Posição da câmera para visualização em perspectiva
    teapot();

}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glPushMatrix();

    viewXY();
    viewXZ();
    viewYZ();
    viewPerspectiva();

    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0); // Definindo a perspectiva
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void update(int value) {
    if (rotateTeapot) { // Verifica se a rotação está ativada
        // Atualizando os valores para as animações
        teapotAngle += 1.0;
        if (teapotAngle > 360) {
            teapotAngle -= 360;
        }
        glutPostRedisplay(); // Chamando a função de display
        glutTimerFunc(16, update, 0); // Configurando o temporizador para chamar a função update a cada 16ms (aproximadamente 60fps)
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'x':
    case 'X':
        rotateTeapot = true; // Inicia a rotação
        glutTimerFunc(25, update, 0); // Iniciando o temporizador
        break;
    case 'c':
    case 'C':
        rotateTeapot = false; // Para a rotação
        break;
    }
    glutPostRedisplay(); // Certifique-se de redesenhar a cena após alterar a rotação
}
