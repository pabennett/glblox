////////////////////////////////////////////////////////////////////////////////
// Player          A class describing the player.
//
// 
// @author         Peter A Bennett
// @copyright      (c) 2012 Peter A Bennett
// @license        LGPL      
// @email          pab850@googlemail.com
// @contact        www.bytebash.com
//
////////////////////////////////////////////////////////////////////////////////

#include "player.hpp"

Player::Player(Camera* camera, World* world)
{              
   orientation = glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);

   Player::world = world;
   playerCamera = camera;
   
   vector3i worldDim = world->worldDimensions();
   int chunk_size = world->worldChunkSize();
   
   position.x = (worldDim.x/2) * chunk_size;
   position.y = (worldDim.y/2) * chunk_size;
   position.z = (worldDim.z/2) * chunk_size;

   playerCamera->setPos(position.x, position.y, position.z);
   glm::vec3 playerBBox = glm::vec3(1.0f, 1.0f, 1.0f);
}

Player::~Player()
{

}

// Perform a collision test on the world and move the player.

/*
Take the unit surface normal of the colliding voxel (pointing outward).
Multiply it by the dot product of itself and the player velocity.
Subtract it from the player's velocity.
*/

void Player::move(float dx, float dy, float dz)
{
   glm::vec3 oldPosition = position;
   glm::vec3 surfaceNormal;
   position = playerCamera->move(dx, dy, dz);
   // Check to see if this new position lies inside a solid voxel:
   
   // // Check BBox front:
   // for(int x = playerBBox.x0; x < playerBBox.x1; x++)
   // {
      // for(int y = playerBBox.y0; y < playerBBox.y1; y++)
      // {
         // if(world->is_solid(position.x + x, position.y + y, position.z)
         // {
            // surfaceNormal = glm::vec3(0,0,1);
            // collisionFront = true;
            // break;
         // }
      // }
      // if(collisionFront)
      // {
         // break;
      // }
   // }
   
   // if(collisionFront)
   // {
      // playerVelocity -= glm::dot(playerVelocity,surfaceNormal);
   // }
}

// Physics update for player
void Player::update(float dt,
                    bool moveForward,
                    bool moveBackward,
                    bool moveLeft,
                    bool moveRight)
{
       
   float movementSpeed = dt * 80;
   float damping = 5.0;
  
   if(moveRight)
   {
      velocity += playerCamera->getCameraRight() * 
                  movementSpeed;
   }
   else if(moveLeft)
   {
      velocity -= playerCamera->getCameraRight() * 
                  movementSpeed;  
   }
   //velocity += playerCamera->getCameraUp() * 
   //            movementSpeed;
      
   if(moveForward)
   {
      velocity += playerCamera->getCameraForward() * 
                  movementSpeed;
   }
   else if(moveBackward)
   {
      velocity -= playerCamera->getCameraForward() * 
                  movementSpeed;
   }
   
   velocity -= velocity * dt * damping;
   
   position += velocity * dt;
   playerCamera->setPos(position.x, position.y, position.z);
}

float Player::getPlayerVelocityX()
{
   return velocity.x;
}

float Player::getPlayerVelocityY()
{
   return velocity.y;
}

float Player::getPlayerVelocityZ()
{
   return velocity.z;
}

// Orientation is manipulated by moving the mouse.
void Player::orient(float pitch , float yaw)
{
   orientation = playerCamera->orient(pitch, yaw);
}

// Upload the camera MVP to the GPU.
void Player::setCameraMVP()
{
   playerCamera->setMVP();
}

glm::mat4 Player::getPlayerCamMVP()
{
   return playerCamera->mvp;
}

glm::vec3 Player::getPosition()
{
   return position;
}

float Player::getPositionX()
{
   return position.x;
}

float Player::getPositionY()
{
   return position.y;
}

float Player::getPositionZ()
{
   return position.z;
}