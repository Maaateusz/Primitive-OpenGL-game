
#ifndef __CAMERAA_H
#define __CAMERAA_H

// -----------------------------------------------
// ----------------------------------------------------------
class CameraAbove : public CCamera
{

public:
    // For FPS/TPS camera
    glm::vec3 shiftUp;
    glm::vec3 shiftBack;

    // C_Mesh *Player = NULL;

    // ---------------------------------------------
    void Init()
    {
        // TO_DO: uzupelnij wedle uznania
        shiftUp = glm::vec3(0.0, 2.0, 0.0);
        shiftBack = glm::vec3(-5.0, 2.0, 0.0);
        // shiftUp = glm::vec3(-5.0, 2.0, 0.0);
        Direction = glm::vec3(1.0, 0.0, 0.0);
        // Player = _player;
    }

    // ---------------------------------------------
    // aktualizacja danych kamery na podstawie
    // danych postaci
    virtual void Update(GLuint _program, glm::vec3 _player_pos, float _player_angle)
    {
        // Kamera z góry
        camPos = glm::vec3(_player_pos.x + _player_angle, 10.0f, _player_pos.z + _player_angle);


        // Angles = glm::vec3(0.0, _player_angle, 0.0);
        // Position = _player_pos;
        // shiftUp.y = tmp;
        // shiftUp.x = tmp;
        // shiftBack.x = tmp + Direction.x;

        // float theta = _player_angle;
        // float distance = 3.0f;
        // float offsetX = distance * glm::radians(theta);
        // float offsetZ = distance * glm::radians(theta);

        // camPos.x = _player_pos.x - offsetX;
        // camPos.z = _player_pos.x - offsetZ;
        // camPos.y = _player_pos.y + distance;

        Position = _player_pos + shiftBack;
        // Position = _player_pos;
        // Position.x += move_x;
        // Position = glm::vec3(move_x, move_y, move_z);

        // CCamera::UpdateDirectionCross();
        CCamera::UpdateMatView();
        CCamera::SendPV(_program);
    }
};

#endif // __CAMERA_H
