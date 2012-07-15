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

Player::Player(Camera* camera, World* world, GLuint program)
{              
   orientation = glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);

   Player::world = world;
   playerCamera = camera;
   
   vector3i worldDim = world->worldDimensions();
   int chunk_size = world->worldChunkSize();
   
   position.x = (worldDim.x/2) * chunk_size;
   position.y = (worldDim.y/2) * chunk_size;
   position.z = (worldDim.z/2) * chunk_size;
   
   bbox = AABB(0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
   bboxSize = 1.0;

   playerCamera->setPos(position.x, position.y, position.z);
   
   // Init VBO
   Player::program = program;
   glGenBuffers(1, &playerVerticesVBO);  
   // Init attributes
	glUseProgram(program);
   posAttrib = glGetAttribLocation(program, "position");
   worldPosAttrib = glGetUniformLocation(program, "worldPosition");
	glUseProgram(0);
   
   // Create player verts.
   vertex v;

   v.x = 1; v.y = 1; v.z = 1; playerVertices.push_back(v); //C 
   v.x = 1; v.y = 1; v.z = 0; playerVertices.push_back(v); //D
   v.x = 0; v.y = 1; v.z = 1; playerVertices.push_back(v); //B
   v.x = 0; v.y = 1; v.z = 1; playerVertices.push_back(v); //B 
   v.x = 1; v.y = 1; v.z = 0; playerVertices.push_back(v); //D
   v.x = 0; v.y = 1; v.z = 0; playerVertices.push_back(v); //A

   v.x = 1; v.y = 0; v.z = 1; playerVertices.push_back(v); //C
   v.x = 0; v.y = 0; v.z = 1; playerVertices.push_back(v); //B
   v.x = 1; v.y = 0; v.z = 0; playerVertices.push_back(v); //D
   v.x = 1; v.y = 0; v.z = 0; playerVertices.push_back(v); //D
   v.x = 0; v.y = 0; v.z = 1; playerVertices.push_back(v); //B
   v.x = 0; v.y = 0; v.z = 0; playerVertices.push_back(v); //A

   v.x = 0; v.y = 1; v.z = 1; playerVertices.push_back(v); //A
   v.x = 0; v.y = 1; v.z = 0; playerVertices.push_back(v); //B
   v.x = 0; v.y = 0; v.z = 1; playerVertices.push_back(v); //D
   v.x = 0; v.y = 0; v.z = 1; playerVertices.push_back(v); //D
   v.x = 0; v.y = 1; v.z = 0; playerVertices.push_back(v); //B
   v.x = 0; v.y = 0; v.z = 0; playerVertices.push_back(v); //C

   v.x = 1; v.y = 1; v.z = 1; playerVertices.push_back(v); //B
   v.x = 1; v.y = 0; v.z = 1; playerVertices.push_back(v); //C
   v.x = 1; v.y = 1; v.z = 0; playerVertices.push_back(v); //A
   v.x = 1; v.y = 1; v.z = 0; playerVertices.push_back(v); //A
   v.x = 1; v.y = 0; v.z = 1; playerVertices.push_back(v); //C
   v.x = 1; v.y = 0; v.z = 0; playerVertices.push_back(v); //D

   v.x = 1; v.y = 1; v.z = 1; playerVertices.push_back(v); //A
   v.x = 0; v.y = 1; v.z = 1; playerVertices.push_back(v); //B
   v.x = 0; v.y = 0; v.z = 1; playerVertices.push_back(v); //C
   v.x = 0; v.y = 0; v.z = 1; playerVertices.push_back(v); //C
   v.x = 1; v.y = 0; v.z = 1; playerVertices.push_back(v); //D
   v.x = 1; v.y = 1; v.z = 1; playerVertices.push_back(v); //A

   v.x = 0; v.y = 0; v.z = 0; playerVertices.push_back(v); //D
   v.x = 0; v.y = 1; v.z = 0; playerVertices.push_back(v); //A
   v.x = 1; v.y = 1; v.z = 0; playerVertices.push_back(v); //B
   v.x = 1; v.y = 1; v.z = 0; playerVertices.push_back(v); //B
   v.x = 1; v.y = 0; v.z = 0; playerVertices.push_back(v); //C
   v.x = 0; v.y = 0; v.z = 0; playerVertices.push_back(v); //D
   
   glBindBuffer(GL_ARRAY_BUFFER, playerVerticesVBO);
   glBufferData(GL_ARRAY_BUFFER, playerVertices.size() * sizeof(vertex), &playerVertices[0], GL_STATIC_DRAW);
}

Player::~Player()
{

}

// void Player::setPlayerBoundingBox(int sizeX, int sizeY, int sizeZ)
// {
   // playerBoundingBox.x = sizeX;
   // playerBoundingBox.y = sizeY;
   // playerBoundingBox.z = sizeZ;
// }

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
}

// Physics update for player
void Player::jump()
{
   velocity.y += 2;
}

void Player::update(float dt,
                    bool moveForward,
                    bool moveBackward,
                    bool moveLeft,
                    bool moveRight)
{
       
   float movementSpeed = dt * 80;
   float gravity = 0.25;
   float damping = 5.0;
   glm::vec3 newPosition;
   glm::vec3 surfaceNormal;
  
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
   
   if(!onGround)
   {
      // Apply the effects of gravity:
      velocity.y -= gravity;
   }
      
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
   
      
   // TODO: This collision detection is hacky and not very good!
   

   
   /*       (px,y1,pz)     (px,py,z1)      
                     |     / 
                  +-------+ 
                 /|      /|
                / |     / |
   (x0,py,pz)  /  |    /  | --(x1,py,pz)        
              +---|---+   | 
              |   +---|---+
              |  /    |  /     p = centre of bounding box (x,y,z)
              | /     | /
              |/      |/
              +-------+
             /      | 
   (px,py,z0)       (px,y0,pz)     
          

      0       1       2         3
   Z+ +-------+-------+-------+
      |       | Z1+-------+   | 
      |       |   | Bound |   |
      |       |   | Box   |   |
      +-------+---|       |---+ 2      voxels (1,2)(1,1)(2,2)(2,1)
      |       | Z0+-------+   |        should be checked.
      |       |   X0  |  X1   |
      |       |       |       |
      +-------+-------+-------+ 1
      |       |       |       |
      |       |       |       |   
      |       |       |       |   
   Z- +-------+-------+-------+ 0
      X-                      X+
   */

   newPosition = position + (velocity * dt); 
   // Check for Top and Botom Bbox hits.
   // BBox is currently fixed at 1x1x1   
   float x0, x1, y0, y1, z0, z1;
   
   vector3i voxCoord(floor(newPosition.x),
                       floor(newPosition.y),
                       floor(newPosition.z));

   // Place the player bounding box over the new position.
   bbox.x0 = newPosition.x-(bboxSize/2);
   bbox.x1 = newPosition.x+(bboxSize/2);
   bbox.y0 = newPosition.y-(bboxSize/2);
   bbox.y1 = newPosition.y+(bboxSize/2);
   bbox.z0 = newPosition.z-(bboxSize/2);
   bbox.z1 = newPosition.z+(bboxSize/2);
   
   // If the player is inside a solid voxel move them up by one unit.
   if(world->is_solid(voxCoord.x, voxCoord.y, voxCoord.z))
   {
      position.y += 1;
      playerCamera->setPos(position.x, position.y, position.z);
   }
   
   // Vector of neighbouring bounding boxes for collision test.
   std::vector<AABB> neighbourBboxes;
   
   // Do we need to check for collisions below?
   if(newPosition.y < position.y or onGround)
   {
      // The player is occupying voxel "voxCoord", test the voxel below this one.
      if(world->is_solid(voxCoord.x, voxCoord.y-1, voxCoord.z))
      {
         neighbourBboxes.push_back(AABB(voxCoord.x, voxCoord.y-1, voxCoord.z,
                                        voxCoord.x+1, voxCoord.y, voxCoord.z+1));
      }
      onGround = false;
      if(AABBCollisionTest(neighbourBboxes, bbox))
      {
         onGround = true;
         surfaceNormal = glm::vec3(0,1,0);
         surfaceNormal = (surfaceNormal * glm::dot(surfaceNormal, velocity));
         velocity -= surfaceNormal;
      }
      neighbourBboxes.clear();
   }
   
   if(newPosition.y > position.y)
   {
      // The player is occupying voxel "voxCoord", test the voxel below this one.
      if(world->is_solid(voxCoord.x, voxCoord.y+1, voxCoord.z))
      {
         neighbourBboxes.push_back(AABB(voxCoord.x, voxCoord.y+1, voxCoord.z,
                                        voxCoord.x+1, voxCoord.y+2, voxCoord.z+1));
      }
      if(AABBCollisionTest(neighbourBboxes, bbox))
      {
         surfaceNormal = glm::vec3(0,-1,0);
         surfaceNormal = (surfaceNormal * glm::dot(surfaceNormal, velocity));
         velocity -= surfaceNormal;
      }
      neighbourBboxes.clear();
   }
   
   if(newPosition.x < position.x)
   {
      // The player is occupying voxel "voxCoord", test the voxel to the left.
      if(world->is_solid(voxCoord.x-1, voxCoord.y, voxCoord.z))
      {
         neighbourBboxes.push_back(AABB(voxCoord.x-1, voxCoord.y, voxCoord.z,
                                        voxCoord.x, voxCoord.y+1, voxCoord.z+1));
      }
      collisionStatusLeft = false;
      if(AABBCollisionTest(neighbourBboxes, bbox))
      {
         collisionStatusLeft = true;
         surfaceNormal = glm::vec3(1,0,0);
         surfaceNormal = (surfaceNormal * glm::dot(surfaceNormal, velocity));
         velocity -= surfaceNormal;
      }
      neighbourBboxes.clear();
   }
   
   if(newPosition.x > position.x)
   {
      // The player is occupying voxel "voxCoord", test the voxel to the right.
      if(world->is_solid(voxCoord.x+1, voxCoord.y, voxCoord.z))
      {
         neighbourBboxes.push_back(AABB(voxCoord.x+1, voxCoord.y, voxCoord.z,
                                        voxCoord.x+2, voxCoord.y+1, voxCoord.z+1));
      }
      collisionStatusRight = false;
      if(AABBCollisionTest(neighbourBboxes, bbox))
      {
         collisionStatusRight = true;
         surfaceNormal = glm::vec3(1,0,0);
         surfaceNormal = (surfaceNormal * glm::dot(surfaceNormal, velocity));
         velocity -= surfaceNormal;
      }
      neighbourBboxes.clear();
   }
   
   if(newPosition.z < position.z)
   {
      // The player is occupying voxel "voxCoord", test the voxel to the back.
      if(world->is_solid(voxCoord.x, voxCoord.y, voxCoord.z-1))
      {
         neighbourBboxes.push_back(AABB(voxCoord.x, voxCoord.y, voxCoord.z-1,
                                        voxCoord.x+1, voxCoord.y+1, voxCoord.z));
      }
      collisionStatusBack = false;
      if(AABBCollisionTest(neighbourBboxes, bbox))
      {
         collisionStatusBack = true;
         surfaceNormal = glm::vec3(0,0,1);
         surfaceNormal = (surfaceNormal * glm::dot(surfaceNormal, velocity));
         velocity -= surfaceNormal;
      }
      neighbourBboxes.clear();
   }
   
   if(newPosition.z > position.z)
   {
      // The player is occupying voxel "voxCoord", test the voxel to the front.
      if(world->is_solid(voxCoord.x, voxCoord.y, voxCoord.z+1))
      {
         neighbourBboxes.push_back(AABB(voxCoord.x, voxCoord.y, voxCoord.z+1,
                                        voxCoord.x+1, voxCoord.y+1, voxCoord.z+2));
      }
      collisionStatusFront = false;
      if(AABBCollisionTest(neighbourBboxes, bbox))
      {
         collisionStatusFront = true;
         surfaceNormal = glm::vec3(0,0,-1);
         surfaceNormal = (surfaceNormal * glm::dot(surfaceNormal, velocity));
         velocity -= surfaceNormal;
      }
      neighbourBboxes.clear();
   }
   
   velocity -= velocity * dt * damping; 
   position += velocity * dt;
   playerCamera->setPos(position.x, position.y, position.z);
   
}

bool Player::getCollisionStatusLeft()
{
   return collisionStatusLeft;
}
bool Player::getCollisionStatusRight()
{
   return collisionStatusRight;
}
bool Player::getCollisionStatusBack()
{
   return collisionStatusBack;
}
bool Player::getCollisionStatusFront()
{
   return collisionStatusFront;
}
bool Player::getCollisionStatusTop()
{
   return collisionStatusTop;
}
bool Player::getCollisionStatusBottom()
{
   return onGround;
}

void Player::draw()
{
   glm::vec3 worldPos;
   worldPos.x = position.x - 0.5;
   worldPos.y = position.y - 0.5;
   worldPos.z = position.z - 0.5;
   
   // Draw the player object
	glUseProgram(program);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glUniform3fv(worldPosAttrib, 1, glm::value_ptr(worldPos));
   glEnableVertexAttribArray(posAttrib);   
   glBindBuffer(GL_ARRAY_BUFFER, playerVerticesVBO);
   glVertexAttribPointer(posAttrib, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
   glDrawArrays(GL_TRIANGLES, 0, playerVertices.size());
   glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glUseProgram(0);
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

bool Player::AABBCollisionTest(std::vector<AABB> neighbourBboxes, AABB bbox)
{
   for(std::vector<AABB>::iterator ii = neighbourBboxes.begin(); ii != neighbourBboxes.end(); ++ii)
   {
      // Check for bounding box overlap (y0)
      AABB voxBox = (*ii);
      if(voxBox.y1 >= bbox.y0)
      {
         if(voxBox.x1 >= bbox.x0 or
            voxBox.z1 >= bbox.z0 or
            voxBox.x0 <= bbox.x1 or
            voxBox.z0 <= bbox.z1)
         {
            // An intersection was found.
            return true;
         }
      }
   }
   return false;
}

