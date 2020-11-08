//
//  Camera.cpp
//  opengl_learn
//
//  Created by chaojie wu on 11/7/20.
//

#include "Camera.h"
#include <glm/gtx/transform.hpp>
#include <iostream>

//--------------------------------------------
// Abstract Camara Class
//--------------------------------------------
Camera::Camera()
    :mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
    mTargetPos(glm::vec3(0.0f, 0.0f, 0.0f)),
    mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    mRight(glm::vec3(0.0f, 0.0f, 0.0f)),
    mYaw(glm::pi<float>()),
    mPitch(0.0f),
    mFOV(45.0f)
{
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(mPosition, mTargetPos, mUp);
}

const glm::vec3& Camera::getLook() const
{
    return mLook;
}

const glm::vec3& Camera::getRight() const
{
    return mRight;
}

const glm::vec3& Camera::getUp() const
{
    return mUp;
}



//--------------------------------------------
// FPS Camara Class
//--------------------------------------------

FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch)
{
    mPosition = position;
    mYaw = yaw;
    mPitch = pitch;
}

void FPSCamera::setPosition(const glm::vec3& position)
{
    mPosition = position;
}

void FPSCamera::move(const glm::vec3& offsetPos)
{
    mPosition += offsetPos;
    updateCameraVectors();
}

void FPSCamera::rotate(float yaw, float pitch)
{

    // Looks like yaw have become radius, but not pitch, do not know why.
    //mYaw = glm::radians(yaw);
    mYaw = yaw;
    mPitch = glm::radians(pitch);
    
    mPitch = glm::clamp(pitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
    updateCameraVectors();
}

// Calculate the vectors from the Camera's (updated) Euler Angle
void FPSCamera::updateCameraVectors()
{
    // Spherical to Cartesian coordinates
    
    // Calculate the view direction vector based on yaw and pitch angles (roll not consider)
    glm::vec3 look;
    look.x = cos(mPitch) * sin(mYaw);
    look.y = sin(mPitch);
    look.z = cos(mPitch) * cos(mYaw);
    
    
    mLook = glm::normalize(look);
    
    // Re-calculate the Right and Up vector.
    mRight = glm::normalize(glm::cross(mLook, WORLD_UP));
    mUp = glm::normalize(glm::cross(mRight, mLook));
    
    mTargetPos = mPosition + mLook;
}



//--------------------------------------------
// Orbit Camara Class
//--------------------------------------------
OrbitCamera::OrbitCamera()
    :mRadius(10.0f)
{
}

void OrbitCamera::setLookAt(const glm::vec3& target)
{
    mTargetPos = target;
}

void OrbitCamera::setRadius(float radius)
{
    mRadius = glm::clamp(radius, 2.0f, 80.f);
}

void OrbitCamera::rotate(float yaw, float pitch)
{
    mYaw = glm::radians(yaw);
    mPitch = glm::radians(pitch);
    
    //mPitch = glm::clamp(pitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
    updateCameraVectors();
}

void OrbitCamera::updateCameraVectors()
{
    mPosition.x = mTargetPos.x + mRadius * cosf(mPitch) * sinf(mYaw);
    mPosition.y = mTargetPos.y + mRadius * sinf(mPitch);
    mPosition.z = mTargetPos.z + mRadius * cosf(mPitch) * cosf(mYaw);
}
