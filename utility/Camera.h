
#ifndef __CAMERA_H
#define __CAMERA_H

// -----------------------------------------------
class CCamera
{
public:
    // Macierze rzutowania i widoku
    glm::mat4 matProj;
    glm::mat4 matView = glm::mat4x4(1.0);

    // Skladowe kontrolujace matView
    glm::vec3 Position;  // polozenie kamery
    glm::vec3 Angles;    // pitch, yaw, roll
    glm::vec3 Up;        // domyslnie wektor (0,1,0)
    glm::vec3 Direction; // wektor kierunku obliczany z Angles
    glm::vec3 camPos;

    // Skladowe kontrolujace matProj
    float Width, Height;       // proporcje bryly obcinania
    float NearPlane, FarPlane; // plaszczyzny tnace
    float Fov;                 // kat widzenia kamery

    // ---------------------------------------------
    CCamera()
    {
        // Domyslne wartosci skladowych
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
        Position = glm::vec3(0.0f, 6.0f, 15.f);
        Angles = glm::vec3(-0.5f, -1.57f, 0.0f);
        Fov = 60.0f;

        NearPlane = 0.1f;
        FarPlane = 300.0f;

        // UpdateDirection();
        UpdateDirectionCross();
    }

    // ---------------------------------------------
    // wirtualna metoda aktualizujaca dane kamery
    // przydatna w klasach pochodnych
    virtual void Update()
    {
    }

    // ---------------------------------------------
    void UpdateMatView()
    {
        matView = glm::lookAt(camPos, Position + Direction, Up);
        // matView = glm::lookAt(Position, Position + Direction, Up);
    }

    // ---------------------------------------------
    void SetPerspective(float width, float height, float fov)
    {
        Width = width;
        Height = height;
        Fov = fov;
        matProj = glm::perspectiveFov(glm::radians(Fov), Width, Height, NearPlane, FarPlane);
    }

    // ---------------------------------------------
    void SetOrtho(float width, float height, float fov)
    {
        Width = width;
        Height = height;
        Fov = fov;
        matProj = glm::perspectiveFov(glm::radians(Fov), width, height, NearPlane, FarPlane);
    }

    // ---------------------------
    void UpdateDirectionCross()
    {
        Direction.x = cos(Angles.y) * cos(Angles.x);
        Direction.y = sin(Angles.x);
        Direction.z = -sin(Angles.y) * cos(Angles.x);
    }

    // ---------------------------------------------
    // przesylanie obu macierzy do programu
    // void SendPV(GLuint program, const char *proj, const char *view)
    // {
    //     glUniformMatrix4fv(glGetUniformLocation(program, proj), 1, GL_FALSE, glm::value_ptr(matProj));
    //     glUniformMatrix4fv(glGetUniformLocation(program, view), 1, GL_FALSE, glm::value_ptr(matView));
    // }

    // ---------------------------------------------
    // przesylanie iloczynu macierzy do programu
    void SendPV(GLuint program)
    {
        glm::vec3 eyePosition = ExtractCameraPos(matView);
        glUniform3fv(glGetUniformLocation(program, "eyePosition"), 1, &eyePosition[0]);

        glm::mat4 matProjView = matProj * matView;
        glUniformMatrix4fv(glGetUniformLocation(program, "matProjView"), 1, GL_FALSE, glm::value_ptr(matProjView));
    }

    // ---------------------------------------------
    void AddFov(GLfloat _fov)
    {
        Fov += _fov;
        SetPerspective(Width, Height, Fov);
    }

    void SetFov(GLfloat _fov)
    {
        Fov = _fov;
        SetPerspective(Width, Height, Fov);
    }

    glm::mat4 getMatProj()
    {
        return matProj;
    }

    glm::mat4 getMatProjView()
    {
        return matProj * matView;
    }
};

// ----------------------------------------------------------
class CFPSCamera : public CCamera
{

public:
    glm::vec3 shiftUp;
    void Init()
    {
        shiftUp = glm::vec3(0.0, 1.5, 0.0);
        // shiftUp = glm::vec3(3.0, 2.0, 0.0);
        Direction = glm::vec3(1.0, 0.0, 0.0);
    }

    virtual void Update(GLuint _program, glm::vec3 _player_pos, float _player_angle)
    {
        camPos = _player_pos + shiftUp;
        Position = camPos;
        Angles = glm::vec3(0.0, _player_angle, 0.0);

        CCamera::UpdateDirectionCross();
        CCamera::UpdateMatView();
        CCamera::SendPV(_program);
    }
};

#endif // __CAMERA_H
