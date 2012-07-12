#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
// OpenGL extension wrangler
#include <GL/glew.h>
#include <GL/wglew.h>

#include <iostream>
#include <cstring>
#include <new>
// OpenGL Mathematics Library
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Implements a quaternion based first person camera.
class Camera
{   
   bool flight_mode;                   // Switch to enable camera flight mode.
   
   glm::vec3 m_xaxis;                  // Camera view matrix x axis
   glm::vec3 m_yaxis;                  // Camera view matrix y axis
   glm::vec3 m_zaxis;                  // Camera view matrix z axis
   
   glm::vec3 forward;                  // Where the camera is pointing.
   glm::vec3 fpforward ;               // First person forwards direction.
   
   int width;                          // The height of the camera view.
   int height;                         // The width of the camera view.
   float aspect;                       // The camera aspect ratio (width/height).
   float fovy;                         // The camera vertical field of view.
   float zNear;                        // The camera view frustrum near clipping plane.
   float zFar;                         // The camera view frustrum far clipping plane.
     
   public:
      glm::vec3 position;
      glm::vec3 rotSpeed;
      glm::vec3 movSpeed;
      
      // Camera matrices
      glm::fquat orientation;             // The camera's current orientation quaternion
      glm::mat4 projection;               // The camera projection (perspective) matrix.
      glm::mat4 view;                     // The camera view matrix.
      glm::mat4 mvp;                      // The camera modelview projection matrix.
      glm::mat4 modelview;                // Modelview matrix.
      Camera(int, int, float, float, float, bool, GLuint);
      void update(float);
      void setMVP();
      void perspective(int, int, float, float, float); 
      void setPos(float, float, float);
      float getPosX();
      float getPosY();
      float getPosZ();
      void orient(float, float);
      void move(float, float, float);
      bool visibleY();
      bool visibleZ();
   private:
      glm::fquat fromAxisAngle(glm::vec3, float);
      void updateView();
      GLuint renderProgram;
};