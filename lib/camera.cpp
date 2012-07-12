////////////////////////////////////////////////////////////////////////////////
// Camera         A class to provide a quaternion based first person or freelook
//                camera. Provides functions to allow the camera to be moved
//                around a 3D scene.
// 
// @author         Peter A Bennett
// @copyright      (c) 2012 Peter A Bennett
// @license        LGPL      
// @email          pab850@googlemail.com
// @contact        www.bytebash.com
//
////////////////////////////////////////////////////////////////////////////////

#include "camera.hpp"

// Global constants
const float PI = atan(1)*4;
const glm::vec3 WORLD_XAXIS = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 WORLD_YAXIS = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 WORLD_ZAXIS = glm::vec3(0.0f, 0.0f, 1.0f);

Camera::Camera(int w, int h, 
               float vertFOV, 
               float frustrumNear, 
               float frustrumFar, 
               bool flightMode,
               GLuint program)
{
   renderProgram = program;
   width = w<=0?1:w;
   height = h<=0?1:h;
   aspect = width / height;
   fovy = vertFOV;
   zNear = frustrumNear;
   zFar = frustrumFar;
   flight_mode = flightMode;
   position = glm::vec3(0.0, 0.0, 0.0);
   rotSpeed = glm::vec3(0.0, 0.0, 0.0);
   movSpeed = glm::vec3(0.0, 0.0, 0.0);
   orientation = glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);
}

// Update the projection matrix.
// Call this function if the viewport changes size.
void Camera::perspective(int w, int h, float vertFOV, float frustrumNear, float frustrumFar)
{
   zFar = frustrumFar;
   zNear = frustrumNear;
   fovy = vertFOV;
   width = w;
   height = h<=0?1:h;
   aspect = (float)width / (float)height;
   // Build the projection matrix.
   projection = glm::perspective(fovy, aspect, zNear, zFar); 
   // Update the view and mvp.
   updateView();
}

// Teleport the camera to the specified world co-ordinates.
void Camera::setPos(float x, float y, float z)
{
   position.x = x;
   position.y = y;
   position.z = z;
   // Update the view and mvp.
   updateView();
}

float Camera::getPosX()
{
   return position.x;
}

float Camera::getPosY()
{
   return position.y;
}

float Camera::getPosZ()
{
   return position.z;
}

// Generates a projection matrix and view matrix and then
// uploads the resulting modelview projection matrix to the mvp
// uniform of the shader program
void Camera::setMVP()
{   
	glUseProgram(renderProgram);
   GLuint mvploc = glGetUniformLocation(renderProgram, "mvp");
   glUniformMatrix4fv(mvploc, 1, GL_FALSE, glm::value_ptr(mvp));
   //GLuint modelViewLoc = glGetUniformLocation(program, "modelview");
   //glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, glm::value_ptr(modelview));
	glUseProgram(0);
}


// Generates a quaternion from the given axis and rotation in degrees.
// Maybe there is a constructor of the quaternion class that does
// this for us automatically? TODO.
glm::fquat Camera::fromAxisAngle(const glm::vec3 axis, float degrees)
{
    float halfTheta = (degrees * (PI / 180.0f))  / 2.0f;
    float s = sinf(halfTheta);
    return glm::fquat(cos(halfTheta), axis.x * s, axis.y * s, axis.z * s);
}

// Orientates the camera by applying the given pitch and yaw values
// in degrees to the camera's current orientation.
void Camera::orient(float pitch , float yaw)
{
   glm::fquat rotation;
   // Apply the x-axis delta to the quaternion.
   // Yaw causes the camera to rotate around the WORLD's Y-axis.
   // We need to left multiply the quaternions because the yaw
   // rotation is in world space.
   if (yaw != 0)
   {  
      rotation = fromAxisAngle(WORLD_XAXIS, yaw);   
      orientation = rotation * orientation;
      orientation = glm::normalize(orientation);
   }
   
   // Apply the y-axis delta to the quaternion.
   // Pitch causes the camera to rotate around the CAMERA's X-axis.
   // We need to right multiply the quaternions because the pitch
   // rotation is in model space.
   if (pitch != 0)
   {
      rotation = fromAxisAngle(WORLD_YAXIS, pitch);  
      orientation = orientation * rotation;
      orientation = glm::normalize(orientation);
   }
   
   updateView();
}

// Moves the camera by the specified x, y, z values.
void Camera::move(float dx, float dy, float dz)
{
   if (flight_mode)
   {
      // This is flight mode so we need to move along the forward axis.
      position += m_xaxis * dx; // Move left or right along the camera's x-axis.
      position += m_yaxis * dy; // Move up or down along the world's y-axis.
      position += forward * dz; // Move forward along the camera's z-axis.
   }
   else
   {
      // Determine the 'forwards' direction (where we are looking). If the camera
      // z-axis is used we will move slower forward as the camera tilts upwards, we
      // instead need to use the axis perpendicular to camera xaxis and world yaxis.
      position += m_xaxis * dx; // Move left or right along the camera's x-axis.
      position += WORLD_YAXIS * dy; // Move up or down along the world's y-axis.
      position += fpforward * dz; // Move forward along the camera's z-axis.
   }
   
   updateView();
}

void Camera::updateView()
{
   // Reconstruct the view matrix.
   view = glm::mat4_cast(orientation);
   
   m_xaxis = glm::vec3(view[0][0], view[1][0], view[2][0]);
   m_yaxis = glm::vec3(view[0][1], view[1][1], view[2][1]);
   m_zaxis = glm::vec3(view[0][2], view[1][2], view[2][2]);
   
   // Apply translation component.
   view[3][0] = -glm::dot(m_xaxis, position);
   view[3][1] = -glm::dot(m_yaxis, position);
   view[3][2] = -glm::dot(m_zaxis, position);
   
   // Determine the 'forwards' direction (where we are looking).
   forward = -m_zaxis;
   fpforward = glm::cross(WORLD_YAXIS, m_xaxis);
   fpforward = glm::normalize(fpforward);
   
   // Reconstruct the MVP matrix.
   modelview = view;
   mvp = projection * modelview;
   
   //std::cout << "CPP: Cam posx = " << position.x << std::endl;
   //std::cout << "CPP: Camera: Forward = " << forward.x << "," << forward.y << "," << forward.z << std::endl;
}
