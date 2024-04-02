#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <png.h>
#include <SOIL/SOIL.h>

// Variáveis para controle de animação
float teapotAngle = 0.0;
float ballPositionY = 0.0;
float ballSpeedY = 0.05; // Velocidade inicial de pulo da bola
float torusRotation = 0.0;
float torusPositionX = 0.0;
float torusSpeedX = 0.02; // Velocidade de movimento lateral do torus
float torusBoundary = 1.2; // Limite de movimento lateral do torus

// Variáveis para controle do zoom
int windowWidth, windowHeight;
float zoomLevel = 1.0;
float zoomRate = 0.1;

GLuint textureID; // Variável para armazenar o ID da textura

void saveScreenshot(const char* filename, int width, int height) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Erro ao abrir o arquivo para salvar o screenshot.\n");
        return;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fprintf(stderr, "Erro ao inicializar a estrutura de escrita PNG.\n");
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fprintf(stderr, "Erro ao inicializar a estrutura de informações PNG.\n");
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);

    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    png_bytep row_pointers[height];
    for (int y = 0; y < height; ++y) {
        row_pointers[y] = (png_bytep)malloc(4 * width);
        glReadPixels(0, height - y - 1, width, 1, GL_RGBA, GL_UNSIGNED_BYTE, row_pointers[y]);
    }

    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, NULL);

    for (int y = 0; y < height; ++y) {
        free(row_pointers[y]);
    }

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    printf("Screenshot salvo como %s\n", filename);
}

void loadTexture() {
    // Carregar a imagem da textura usando a biblioteca SOIL
    textureID = SOIL_load_OGL_texture(
                    "./brickswall.jpg", // Caminho para a imagem da textura
                    SOIL_LOAD_AUTO, // Carregar automaticamente
                    SOIL_CREATE_NEW_ID, // Criar um novo ID de textura
                    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT // Opções de carregamento
                );

    if (!textureID) {
        printf("Erro ao carregar a textura: %s\n", SOIL_last_result());
    }

    // Definir os parâmetros de filtragem e repetição da textura
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void updateProjectionMatrix(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0 / zoomLevel, (GLfloat)width / (GLfloat)height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '+':
        zoomLevel += zoomRate;
        updateProjectionMatrix(windowWidth, windowHeight);
        break;
    case '-':
        if (zoomLevel > zoomRate) {
            zoomLevel -= zoomRate;
            updateProjectionMatrix(windowWidth, windowHeight);
        }
        break;
    case 's':
    case 'S':
        saveScreenshot("screenshot.png", windowWidth, windowHeight);
        break;
    }
    glutPostRedisplay(); // Certifique-se de redesenhar a cena após alterar o zoom
}


void init(void) {
    // Configuração da luz
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 }; // Iluminação ambiente
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; // Iluminação difusa
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // Iluminação especular
    GLfloat light_position[] = { 0.0, 1.0, 0.0, 0.0 }; // Luz apontando para baixo
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glClearColor(0.5, 0.5, 0.5, 0.0);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST); // Remoção de superfícies ocultas
    glEnable(GL_TEXTURE_2D);

    glutKeyboardFunc(keyboard);
}

void scenery() {
    // Definindo a cor vermelha para as paredes e chão
    GLfloat mat_red[] = { 1.0, 0.0, 0.0, 1.0 };

    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_red);

    // Chão
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(-2.0, -1.0, -2.0);
    glVertex3f(-2.0, -1.0, 2.0);
    glVertex3f(2.0, -1.0, 2.0);
    glVertex3f(2.0, -1.0, -2.0);
    glEnd();

    // Paredes
    glBindTexture(GL_TEXTURE_2D, textureID); // Vincular a textura
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-2.0, -1.0, -2.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(2.0, -1.0, -2.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(2.0, 0.1, -2.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-2.0, 0.1, -2.0);

    glTexCoord2f(0.0, 0.0);
    glVertex3f(2.0, -1.0, -2.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(2.0, -1.0, 2.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(2.0, 0.1, 2.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(2.0, 0.1, -2.0);
    glEnd();

    glPopMatrix();
}

void teapot() {
    // Definindo a cor azul para o bule
    GLfloat blue_mat_ambient[] = { 0.3, 0.7, 1.0, 0.0 }; // Azul para a cor ambiente
    GLfloat blue_mat_diffuse[] = { 0.3, 0.7, 1.0, 0.0 }; // Azul para a cor difusa
    //Definindo a aparencia do material do objeto
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat high_shininess[] = { 100.0 };

    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, blue_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    // Desvincular qualquer textura que possa estar vinculada
    glBindTexture(GL_TEXTURE_2D, 0);

    glTranslatef (-1.0, 0.0, -0.5);
    glRotatef(teapotAngle, 0.0, 1.0, 0.0);
    glutSolidTeapot(0.6);
    glPopMatrix();
}

void ball() {
    // Definindo a cor amarela para a bola
    GLfloat yellow_mat_ambient[] = { 1.0, 1.0, 0.0, 1.0 }; // Amarelo para a cor ambiente
    GLfloat yellow_mat_diffuse[] = { 1.0, 1.0, 0.0, 1.0 }; // Amarelo para a cor difusa

    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, yellow_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_mat_diffuse);

    // Desvincular qualquer textura que possa estar vinculada
    glBindTexture(GL_TEXTURE_2D, 0);

    glTranslatef (0.6, 0.0, -0.7);
    glTranslatef (0.0, ballPositionY, 0.0); // Aplica o deslocamento vertical da bola
    glutSolidSphere (0.6, 20, 16);
    glPopMatrix();
}

void torus() {
    // Definindo a cor verde para o torus
    GLfloat green_mat_ambient[] = { 0.0, 1.0, 0.0, 1.0 }; // Verde para a cor ambiente
    GLfloat green_mat_diffuse[] = { 0.0, 1.0, 0.0, 1.0 }; // Verde para a cor difusa
    //Definindo a aparencia do material do objeto
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat high_shininess[] = { 100.0 };

    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, green_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, green_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    // Desvincular qualquer textura que possa estar vinculada
    glBindTexture(GL_TEXTURE_2D, 0);

    glRotatef (270.0, 1.0, 0.0, 0.0);
    glTranslatef (torusPositionX, -1.15, -0.8); // Modificando a posição lateral do torus
    glutSolidTorus (0.200, 0.700, 8, 100);
    glPopMatrix();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Configurando a câmera
    gluLookAt(-7.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    scenery();
    teapot();
    ball();
    torus();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    windowWidth = w; // Atualiza a largura da janela
    windowHeight = h; // Atualiza a altura da janela
    glViewport(0, 0, w, h);
    updateProjectionMatrix(w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0); // Definindo a perspectiva
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void update(int value) {
    // Atualizando os valores para as animações
    teapotAngle += 1.0;
    if (teapotAngle > 360) {
        teapotAngle -= 360;
    }

    // Atualizando a posição vertical da bola para simular pulo
    ballPositionY += ballSpeedY;
    if (ballPositionY > 0.6) {
        ballSpeedY = -0.05; // Inverte a direção do pulo ao atingir o topo
    } else if (ballPositionY < 0.0) {
        ballPositionY = 0.0;
        ballSpeedY = 0.05; // Reinicia o pulo quando atinge o chão
    }

    // Atualizando a posição lateral do torus
    torusPositionX += torusSpeedX;
    if (torusPositionX > torusBoundary || torusPositionX < -torusBoundary) {
        torusSpeedX *= -1.0; // Inverte a direção do movimento ao atingir o limite
    }

    glutPostRedisplay(); // Chamando a função de display
    glutTimerFunc(16, update, 0); // Configurando o temporizador para chamar a função update a cada 16ms (aproximadamente 60fps)
}









