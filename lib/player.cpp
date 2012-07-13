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

Player::Player(float x, float y, float z, Camera* camera)
{              
   position.x = x;
   position.y = y;
   position.z = z;
   orientation = glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);
   playerCamera = camera;
   playerCamera->setPos(position.x, position.y, position.z);
}

Player::~Player()
{

}

void Player::move(float dx, float dy, float dz)
{
   position = playerCamera->move(dx, dy, dz);
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