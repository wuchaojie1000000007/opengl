//
//  Camera.hpp
//  opengl_learn
//
//  Created by chaojie wu on 11/7/20.
//

#ifndef Camera_h
#define Camera_h

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

//--------------------------------------------
// Abstract Camara Class
//--------------------------------------------
class Camera
{
public:
    
    glm::mat4 getViewMatrix() const;
    
    virtual void setPosition(const glm::vec3& position) {}
    virtual void rotate(float yaw, float pitch) {} // in degrees
    virtual void move(const glm::vec3& offsetPos) {}
    
    const glm::vec3& getLook() const;
    const glm::vec3& getRight() const;
    const glm::vec3& getUp() const;
    
    float getFOV() const { return mFOV; }
    void setFOV(float fov) { mFOV = fov; } // in degree
    
protected:
    Camera();
    
    virtual void updateCameraVectors() {}
    
    // Member variable
    glm::vec3 mPosition;
    glm::vec3 mTargetPos;
    glm::vec3 mUp;
    glm::vec3 mLook;
    glm::vec3 mRight;
    const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
    
    // Eulers angles (in radians)
    float mYaw;
    float mPitch;
    
    // Camera parameters
    float mFOV;
};

//--------------------------------------------
// FPS Camara Class
//--------------------------------------------
class FPSCamera : public Camera
{
public:
    
    FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = glm::pi<float>(), float pitch = 0.0f);

    virtual void setPosition(const glm::vec3& position);
    virtual void rotate(float yaw, float pitch);    // in degree
    virtual void move(const glm::vec3& offsetPos);

private:
    void updateCameraVectors();
};

//--------------------------------------------
// Orbit Camara Class
//--------------------------------------------
class OrbitCamera: public Camera
{
public:
    OrbitCamera();
    
    // Can use mouse
    virtual void rotate(float yaw, float pitch); // in degrees
    
    void setLookAt(const glm::vec3& target);
    void setRadius(float radius);

private:
    
    void updateCameraVectors();
    float mRadius;
};


#endif /* Camera_h */
