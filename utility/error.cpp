#include <GL/glew.h>
#include <GL/freeglut.h>

#define __CHECK_FOR_ERRORS                                                                                            \
    {                                                                                                                 \
        GLenum errCode;                                                                                               \
        if ((errCode = glGetError()) != GL_NO_ERROR)                                                                  \
            printf("Error (%d): %s in file %s at line %d !\n", errCode, gluErrorString(errCode), __FILE__, __LINE__); \
    }
