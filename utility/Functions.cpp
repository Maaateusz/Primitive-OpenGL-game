// --------------- C++ ---------------

// --------------- OpenGL ---------------
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// --------------- Inne ---------------
int fps = 0;

// --------------- Animacje ---------------
float tree_rotate_anim = 0.0f;
float tree_2_rotate_anim = 0.0f;
float light_rotate_anim = 0.0f;
int frame = 0;
float snowflake_anim = 0.0f;
float flowers_anim = 0.0f;
float cat_rotation = 0.0f;
float water_anim = 0.0f;
float particles_anim = 0.0f;

// --------------- Menu ---------------
int light_mode = 3;

// --------------- Macierze ---------------
glm::mat4x4 Matrix_rotate_X(float _angle)
{
    glm::mat4x4 Matrix_rotate_X;
    Matrix_rotate_X[0] = glm::vec4(1, 0, 0, 0);
    Matrix_rotate_X[1] = glm::vec4(0, cos(_angle), -sin(_angle), 0);
    Matrix_rotate_X[2] = glm::vec4(0, sin(_angle), cos(_angle), 0);
    Matrix_rotate_X[3] = glm::vec4(0, 0, 0, 1);
    return Matrix_rotate_X;
}

glm::mat4x4 Matrix_rotate_Y(float _angle)
{
    glm::mat4x4 Matrix_rotate_Y;
    Matrix_rotate_Y[0] = glm::vec4(cos(_angle), 0, sin(_angle), 0);
    Matrix_rotate_Y[1] = glm::vec4(0, 1, 0, 0);
    Matrix_rotate_Y[2] = glm::vec4(-sin(_angle), 0, cos(_angle), 0);
    Matrix_rotate_Y[3] = glm::vec4(0, 0, 0, 1);
    return Matrix_rotate_Y;
}

glm::mat4x4 Matrix_rotate_Z(float _angle)
{
    glm::mat4x4 Matrix_rotate_Z;
    Matrix_rotate_Z[0] = glm::vec4(cos(_angle), sin(_angle), 0, 0);
    Matrix_rotate_Z[1] = glm::vec4(-sin(_angle), cos(_angle), 0, 0);
    Matrix_rotate_Z[2] = glm::vec4(0, 0, 1, 0);
    Matrix_rotate_Z[3] = glm::vec4(0, 0, 0, 1);
    return Matrix_rotate_Z;
}

glm::mat4x4 Matrix_rotate(float _angle, glm::vec3 _vec)
{
    glm::mat4x4 Matrix_rotate;
    Matrix_rotate[0] = glm::vec4(_vec.x * _vec.x * (1 - cos(_angle)) + cos(_angle), _vec.x * _vec.y * (1 - cos(_angle)) - (_vec.z * sin(_angle)), _vec.x * _vec.z * (1 - cos(_angle)) + (_vec.y * sin(_angle)), 0.0f);
    Matrix_rotate[1] = glm::vec4(_vec.x * _vec.y * (1 - cos(_angle)) + _vec.z * sin(_angle), _vec.y * _vec.y * (1 - cos(_angle)) + (cos(_angle)), _vec.y * _vec.z * (1 - cos(_angle)) - (_vec.x * sin(_angle)), 0.0f);
    Matrix_rotate[2] = glm::vec4(_vec.x * _vec.z * (1 - cos(_angle)) - _vec.y * sin(_angle), _vec.z * _vec.y * (1 - cos(_angle)) + (_vec.x * sin(_angle)), _vec.z * _vec.z * (1 - cos(_angle)) + (cos(_angle)), 0.0f);
    // Matrix_rotate[2] = glm::vec4(_vec.x * _vec.z * (1 - cos(_angle)) - _vec.y *sin(_angle), _vec.z * _vec.y * (1 - cos(_angle)) + (_vec.z * sin(_angle)), _vec.z * _vec.z * (1 - cos(_angle)) + (cos(_angle)), 0.0f);
    Matrix_rotate[3] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    return Matrix_rotate;
}

// ---------------------------------------

GLuint createTexture(const char *filename)
{
    int tex_width, tex_height, n;
    unsigned char *tex_data;
    stbi_set_flip_vertically_on_load(true);

    tex_data = stbi_load(filename, &tex_width, &tex_height, &n, 0);
    if (tex_data == NULL)
    {
        printf("Image can't be loaded! => %s \n", filename);
        exit(1);
    }

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    if (n == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
    else if (n == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(tex_data);

    return id;
}

// https://stackoverflow.com/questions/5627803/how-to-calculate-fps-in-opengl
void CalculateFrameRate()
{
    static float framesPerSecond = 0.0f; // This will store our fps
    static float lastTime = 0.0f;        // This will hold the time from the last frame
    float currentTime = GetTickCount() * 0.001f;
    ++framesPerSecond;
    if (currentTime - lastTime > 1.0f)
    {
        lastTime = currentTime;
        fps = int(framesPerSecond);
        framesPerSecond = 0;
    }
}

void Animation(int keyframe)
{
    frame++;
    tree_rotate_anim += 0.1f;
    // tree_2_rotate_anim += 7.0f;
    light_rotate_anim += 0.07f;
    particles_anim += 0.1f;
    // snowflake_anim += 0.8f;
    flowers_anim += 0.1f;
    cat_rotation += 1.0f;
    water_anim += 0.003f;

    glutTimerFunc(50, Animation, 0);
    glutPostRedisplay();
}

void GlutAppInit(int argc, char *argv[])
{
    // MSAA
    // glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitWindowSize(1280, 740);
    glutCreateWindow("Main - OpenGL");
}

void GlewAppInit()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        printf("GLEW Error\n");
        exit(1);
    }
}

void OpenGlAppInit()
{
    if (!GLEW_VERSION_3_2)
    {
        printf("Brak OpenGL 3.2!\n");
        exit(1);
    }
}

void Menu(int value)
{
    switch (value)
    {
    case 1:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case 2:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case 3:
        light_mode = 1;
        printf("No light\n");
        break;
    case 4:
        light_mode = 2;
        printf("Point light + Reflections\n");
        break;
    case 5:
        light_mode = 3;
        printf("Point light + Reflections + Shadow\n");
        break;
    default:
        printf("Wybrano %d \n", value);
    }
}

void MenuInit()
{
    // Utworzenie glownego menu kontekstowego
    glutCreateMenu(Menu);

    glutAddMenuEntry("Show objects", 1);
    glutAddMenuEntry("Show mesh", 2);
    glutAddMenuEntry("----------------", 2137);
    glutAddMenuEntry("No light", 3);
    glutAddMenuEntry("Point light + Reflections", 4);
    glutAddMenuEntry("Point light + Reflections + Shadow", 5);

    // Przypisanie klawisza myszy uruchamiajacego menu
    // GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, and GLUT_RIGHT_BUTTON.
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
