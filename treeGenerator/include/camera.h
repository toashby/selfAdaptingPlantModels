///@brief A camera class
/// parts of this code are modified from :-
/// https://learnopengl.com/

#ifndef CAMERA_H
#define CAMERA_H

//#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  10.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


/// \author T ASHBY
/// \version 1.0
/// \date Last Revision 18/08/19 \n

/// \class camera.h
/// \brief An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
/// \todo
class Camera
{
public:
    // Camera Attributes
    /// @brief position in world space
    glm::vec3 Position;
    /// @brief forward vector
    glm::vec3 Front;
    /// @brief up vector
    glm::vec3 Up;
    /// @brief right vector
    glm::vec3 Right;
    /// @brief world up vector
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructor with vectors
    /// @brief Camera constructor taking vector values
    /// @param[in] _position The camera position in global space
    /// @param[in] _up The camera up direction
    /// @param[in] _yaw The camera yaw
    /// @param[in] _pitch The camera pitch
    Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f), float _yaw = YAW, float _pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = _position;
        WorldUp = _up;
        Yaw = _yaw;
        Pitch = _pitch;
        updateCameraVectors();
    }
    // Constructor with scalar values
    /// @brief Camera constructor taking scalar values
    /// @param[in] _posX The camera position in global space
    /// @param[in] _posY The camera up direction
    /// @param[in] _posZ The camera yaw
    /// @param[in] _upX The camera pitch
    /// @param[in] _upY The camera position in global space
    /// @param[in] _upZ The camera up direction
    /// @param[in] _yaw The camera yaw
    /// @param[in] _pitch The camera pitch
    Camera(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(_posX, _posY, _posZ);
        WorldUp = glm::vec3(_upX, _upY, _upZ);
        Yaw = _yaw;
        Pitch = _pitch;
        updateCameraVectors();
    }

    /// @brief Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    ///@brief Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    /// @param[in] _direction The direction
    /// @param[in] _deltaTime The time since last frame
    void ProcessKeyboard(Camera_Movement _direction, float _deltaTime)
    {
        float velocity = MovementSpeed * _deltaTime;
        if (_direction == FORWARD)
            Position += Front * velocity;
        if (_direction == BACKWARD)
            Position -= Front * velocity;
        if (_direction == LEFT)
            Position -= Right * velocity;
        if (_direction == RIGHT)
            Position += Right * velocity;
    }

    /// @brief Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    /// @param[in] _xoffset The x offset
    /// @param[in] _yoffset The y offset
    /// @param[in] _constrainPitch Pitch constraint boolean
    void ProcessMouseMovement(float _xoffset, float _yoffset, GLboolean _constrainPitch = true)
    {
        _xoffset *= MouseSensitivity;
        _yoffset *= MouseSensitivity;

        Yaw   += _xoffset;
        Pitch += _yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (_constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    /// @brief Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    /// @param[in] _yoffset The offset in the y direction
    void ProcessMouseScroll(float _yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= _yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

private:
    /// @brief Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif
