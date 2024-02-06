// transforma.h
#ifndef TRANSFORMA_H
#define TRANSFORMA_H

#include <GL/gl.h>
#include <vector>
#include <functional>

// Declaração do tipo Ray
struct Ray {
    GLfloat origin[3]; // Origem do raio (x, y, z)
    GLfloat direction[3]; // Direção do raio (x, y, z)

    // Construtor para inicialização dos membros
    Ray(const GLfloat o[3], const GLfloat d[3]) {
        for (int i = 0; i < 3; ++i) {
            origin[i] = o[i];
            direction[i] = d[i];
        }
    }
};

Ray createRayFromClick(int x, int y, const GLfloat* projection, const GLfloat* modelview);
float intersectRayWithObject(const Ray& ray, const std::function<void()>& renderFunction);


extern bool isSelectionMode, isTransforming, isRotationPerformed;
extern int selectedObjectIndex;

void selecionarObjeto(int x, int y, const std::vector<std::function<void()>>& functionsToCall);
void calcularLimitesObjeto(const std::function<void()>& renderFunction, GLfloat& minX, GLfloat& minY, GLfloat& maxX, GLfloat& maxY);
void updateLimits(GLdouble x, GLdouble y, GLfloat& minX, GLfloat& minY, GLfloat& maxX, GLfloat& maxY);
void excluirObjeto();
void transladarObjeto(float translateX, float translateY);
void rotacionaObjeto(float angleDegrees);
void escalaObjeto(float scaleX, float scaleY);
void cisalhaObjeto(float shearX, float shearY);
void refleteObjeto(bool refletirX, bool refletirY);

#endif // TRANSFORMA_H
