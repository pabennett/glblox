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
   
   position.x = float((worldDim.x/2) * chunk_size);
   position.y = float((worldDim.y/2) * chunk_size);
   position.z = float((worldDim.z/2) * chunk_size);
   
   voxelSize = 1.0f;
   
   gravityEnabled = true;
   flightEnabled = false;
   collisionTestsEnabled = true;
   
   playerCamera->setFlightMode(flightEnabled);
   
   /*
   Bounding box is 2x1:
   
      y1 +-------+ ---
         |       |  |
         |   C   |  |    <-- Camera located in upper quarter of box.
         |       |  |
         +-------+  | 
         |       |  |
         |       |  |                       
         |       |  |
      y0 +-------+ ---
         x0      x1         
   */
   // Create a player bounding box of dims 0.25x2x0.25
   // Box is positioned at camera.y - 0.5
   playerBox = AABB(glm::vec3(position.x, position.y-0.5f, position.z),
                    glm::vec3(0.25f, 1.0f, 0.25f));

   playerCamera->setPos(position.x, position.y, position.z);
   
   // Will be used later for rendering player objects.
   Player::program = program;

}

Player::~Player()
{

}

void Player::move(float dx, float dy, float dz)
{
   glm::vec3 oldPosition = position;
   position = playerCamera->move(dx, dy, dz);
   playerBox.setPosition(glm::vec3(position.x, position.y-0.5f, position.z));
}

// Enable or disable flight mode
void Player::setFlightMode(bool flightEnabled)
{
   Player::flightEnabled = flightEnabled;
   playerCamera->setFlightMode(flightEnabled);
   gravityEnabled = not flightEnabled;
}
   
// Jump!
void Player::jump()
{
   if(onGround)
   {
      velocity.y += 35.0f;
   }
}

//Shoot!
void Player::fire(int mode)
{
   glm::vec3 pVelocity = playerCamera->getCameraForward() * 150.0f;
   projectiles.push_back(new Projectile(world, position, pVelocity, program, mode));
}

// Physics update for player
void Player::update(float dt,
                    bool moveForward,
                    bool moveBackward,
                    bool moveLeft,
                    bool moveRight)
{
       
   float movementSpeed = dt * 80.0f;
   float gravity = 120.0f;
   float damping = 8.0f;
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
   
   if(!onGround and gravityEnabled)
   {
      // Apply the effects of gravity:
      velocity.y -= gravity * dt;
   }
         
   if(moveForward)
   {
      if(flightEnabled)
      {
         velocity += playerCamera->getCameraForward() * 
                     movementSpeed;
      }
      else
      {
         velocity += playerCamera->getCameraFPForward() * 
                     movementSpeed;
      }
   }
   else if(moveBackward)
   {
      if(flightEnabled)
      {
         velocity -= playerCamera->getCameraForward() * 
                     movementSpeed;
      }
      else
      {
         velocity -= playerCamera->getCameraFPForward() * 
                     movementSpeed;
      }
   }
   
   /* Collision Test */

   //Take the unit surface normal of the colliding voxel (pointing outward).
   //Multiply it by the dot product of itself and the player velocity.
   //Subtract it from the player's velocity.

   /*       (px,y1,pz)     (px,py,z1)      
                     |     / 
                  +-------+ 
                 /|      /|
                / |     / |
   (x0,py,pz)  /  |    /  | --(x1,py,pz)        
              +---|---+   | 
              |   +---|---+
              |  /    |  /     p = centre of bounding box (x,y,z)
      +Z      | /     | /
 +Y| /        |/      |/
   |/         +-------+
   +---+X    /      | 
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

   if(collisionTestsEnabled)
   {
      // Update player bounding box.
      playerBox.setPosition(glm::vec3(position.x, position.y-0.5f, position.z));
           
      // Do we need to check for collisions below?
      int vx, vy, vz; 
      // Sz = Voxel Size / 2 = 0.5
      const float sz = voxelSize / 2.0f; 
      
      // Falling or touching ground.
      if(velocity.y < 0.0f or onGround)
      {
         onGround = false;
         if(bottomCollisionTest())
         {
            onGround = true;
            if(velocity.y < 0.0f)
            {
               surfaceNormal = glm::vec3(0.0f,1.0f,0.0f);
               surfaceNormal = (surfaceNormal * glm::dot(surfaceNormal, velocity));
               velocity -= surfaceNormal;
            }
         }
      }
      // Upper collision test.
      else if(velocity.y > 0.0f)
      {
         if(topCollisionTest())
         {
            surfaceNormal = glm::vec3(0.0f,-1.0f,0.0f);
            surfaceNormal = (surfaceNormal * glm::dot(surfaceNormal, velocity));
            velocity -= surfaceNormal;
         }
      }
      // Left collision test
      if(velocity.x < 0.0f)
      {
         if(leftCollisionTest())
         {
            surfaceNormal = glm::vec3(1.0f,0.0f,0.0f);
            surfaceNormal = (surfaceNormal * glm::dot(surfaceNormal, velocity));
            velocity -= surfaceNormal;
         }
      }
      // Right collision test
      else if(velocity.x > 0.0f)
      {
         if(rightCollisionTest())
         {
            surfaceNormal = glm::vec3(-1.0f,0.0f,0.0f);
            surfaceNormal = (surfaceNormal * glm::dot(surfaceNormal, velocity));
            velocity -= surfaceNormal;
         }
      }
      // Back collision test
      if(velocity.z < 0.0f)
      {
         if(backCollisionTest())
         {
            surfaceNormal = glm::vec3(0.0f,0.0f,1.0f);
            surfaceNormal = (surfaceNormal * glm::dot(surfaceNormal, velocity));
            velocity -= surfaceNormal;
         }
      }
      // Front collision test
      else if(velocity.z > 0.0f)
      {
         if(frontCollisionTest())
         {
            surfaceNormal = glm::vec3(0.0f,0.0f,-1.0f);
            surfaceNormal = (surfaceNormal * glm::dot(surfaceNormal, velocity));
            velocity -= surfaceNormal;
         }
      }
   }
   
   velocity -= velocity * dt * damping; 
   position += velocity * dt;
   playerCamera->setPos(position.x, position.y, position.z);
   
   // Update projectiles
   if(!projectiles.empty())
   {
      for(std::vector<Projectile*>::iterator ii = projectiles.begin(); 
                                             ii != projectiles.end();)
      {
         Projectile* projectile = (*ii);
         projectile->update(dt);
         if(projectile->stopped())
         {
            ii = projectiles.erase(ii);
            delete projectile;
         }
         else
         {
            ++ii;
         }
      }
   }
}

void Player::draw()
{
   if(!projectiles.empty())
   {
      for(std::vector<Projectile*>::iterator ii = projectiles.begin(); 
                                             ii != projectiles.end(); ++ii)
      {
         (*ii)->draw();
      }
   }
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
// Arguments are in radians
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

bool Player::bottomCollisionTest()
{
   int vx, vy, vz;
   
   vx = floor(playerBox.min.x);
   vy = floor(playerBox.min.y);
   vz = floor(playerBox.min.z);

   if(touchingVoxelBelowAt(vx, vy, vz)) return true;

   vx = floor(playerBox.max.x);
   vy = floor(playerBox.min.y);
   vz = floor(playerBox.min.z);

   if(touchingVoxelBelowAt(vx, vy, vz)) return true;
   
   vx = floor(playerBox.min.x);
   vy = floor(playerBox.min.y);
   vz = floor(playerBox.max.z);

   if(touchingVoxelBelowAt(vx, vy, vz)) return true;
   
   vx = floor(playerBox.max.x);
   vy = floor(playerBox.min.y);
   vz = floor(playerBox.max.z);

   if(touchingVoxelBelowAt(vx, vy, vz)) return true;
   
   return false;
}

bool Player::topCollisionTest()
{
   int vx, vy, vz;
   
   vx = floor(playerBox.min.x);
   vy = floor(playerBox.max.y);
   vz = floor(playerBox.min.z);

   if(touchingVoxelAboveAt(vx, vy, vz)) return true;

   vx = floor(playerBox.max.x);
   vy = floor(playerBox.max.y);
   vz = floor(playerBox.min.z);

   if(touchingVoxelAboveAt(vx, vy, vz)) return true;
   
   vx = floor(playerBox.min.x);
   vy = floor(playerBox.max.y);
   vz = floor(playerBox.max.z);

   if(touchingVoxelAboveAt(vx, vy, vz)) return true;
   
   vx = floor(playerBox.max.x);
   vy = floor(playerBox.max.y);
   vz = floor(playerBox.max.z);

   if(touchingVoxelAboveAt(vx, vy, vz)) return true;
   
   return false;
}

bool Player::leftCollisionTest()
{
   int vx, vy, vz;

   vx = floor(playerBox.min.x);
   vy = floor(playerBox.centre.y);
   vz = floor(playerBox.min.z);
   
   if(touchingVoxelLeftAt(vx, vy, vz)) return true;
   
   vx = floor(playerBox.min.x);
   vy = floor(playerBox.max.y);
   vz = floor(playerBox.min.z);
   
   if(touchingVoxelLeftAt(vx, vy, vz)) return true;
   
   vx = floor(playerBox.min.x);
   vy = floor(playerBox.centre.y);
   vz = floor(playerBox.max.z);
   
   if(touchingVoxelLeftAt(vx, vy, vz)) return true;
   
   vx = floor(playerBox.min.x);
   vy = floor(playerBox.max.y);
   vz = floor(playerBox.max.z);
   
   if(touchingVoxelLeftAt(vx, vy, vz)) return true;

   return false;
}

bool Player::rightCollisionTest()
{
   int vx, vy, vz;

   vx = floor(playerBox.max.x);
   vy = floor(playerBox.centre.y);
   vz = floor(playerBox.min.z);
   
   if(touchingVoxelRightAt(vx, vy, vz)) return true;
   
   vx = floor(playerBox.max.x);
   vy = floor(playerBox.max.y);
   vz = floor(playerBox.min.z);
   
   if(touchingVoxelRightAt(vx, vy, vz)) return true;
   
   vx = floor(playerBox.max.x);
   vy = floor(playerBox.centre.y);
   vz = floor(playerBox.max.z);
   
   if(touchingVoxelRightAt(vx, vy, vz)) return true;
   
   vx = floor(playerBox.max.x);
   vy = floor(playerBox.max.y);
   vz = floor(playerBox.max.z);
   
   if(touchingVoxelRightAt(vx, vy, vz)) return true;

   return false;
}

bool Player::backCollisionTest()
{
   int vx, vy, vz;

   vx = floor(playerBox.min.x);
   vy = floor(playerBox.centre.y);
   vz = floor(playerBox.min.z);
   
   if(touchingVoxelBackAt(vx, vy, vz)) return true;
   
   vx = floor(playerBox.min.x);
   vy = floor(playerBox.max.y);
   vz = floor(playerBox.min.z);
   
   if(touchingVoxelBackAt(vx, vy, vz)) return true;
   
   vx = floor(playerBox.max.x);
   vy = floor(playerBox.centre.y);
   vz = floor(playerBox.min.z);
   
   if(touchingVoxelBackAt(vx, vy, vz)) return true;
   
   vx = floor(playerBox.max.x);
   vy = floor(playerBox.max.y);
   vz = floor(playerBox.min.z);
   
   if(touchingVoxelBackAt(vx, vy, vz)) return true;
   
   return false;
}

bool Player::frontCollisionTest()
{
   int vx, vy, vz;
   
   vx = floor(playerBox.min.x);
   vy = floor(playerBox.centre.y);
   vz = floor(playerBox.max.z);
   
   if(touchingVoxelFrontAt(vx, vy, vz)) return true;
   
   vx = floor(playerBox.min.x);
   vy = floor(playerBox.max.y);
   vz = floor(playerBox.max.z);
   
   if(touchingVoxelFrontAt(vx, vy, vz)) return true;
   
   vx = floor(playerBox.max.x);
   vy = floor(playerBox.centre.y);
   vz = floor(playerBox.max.z);
   
   if(touchingVoxelFrontAt(vx, vy, vz)) return true;
   
   vx = floor(playerBox.max.x);
   vy = floor(playerBox.max.y);
   vz = floor(playerBox.max.z);
   
   if(touchingVoxelFrontAt(vx, vy, vz)) return true;
   
   return false;
}


bool Player::touchingVoxelBelowAt(int x, int y, int z)
{
   const float sz = voxelSize / 2.0f; 
   if(world->is_solid(x, y, z))
   {        
      AABB neighbour(glm::vec3(x+sz, y+sz, z+sz),glm::vec3(sz, sz, sz));
      if(playerBox.bottomCollisionTest(neighbour)) return true;
   }
   return false;
}

bool Player::touchingVoxelAboveAt(int x, int y, int z)
{
   const float sz = voxelSize / 2.0f; 
   if(world->is_solid(x, y, z))
   {        
      AABB neighbour(glm::vec3(x+sz, y+sz, z+sz),glm::vec3(sz, sz, sz));
      if(playerBox.topCollisionTest(neighbour)) return true;
   }
   return false;
}

bool Player::touchingVoxelLeftAt(int x, int y, int z)
{
   const float sz = voxelSize / 2.0f; 
   if(world->is_solid(x, y, z))
   {        
      AABB neighbour(glm::vec3(x+sz, y+sz, z+sz),glm::vec3(sz, sz, sz));
      if(playerBox.leftCollisionTest(neighbour)) return true;
   }
   return false;
}

bool Player::touchingVoxelRightAt(int x, int y, int z)
{
   const float sz = voxelSize / 2.0f; 
   if(world->is_solid(x, y, z))
   {        
      AABB neighbour(glm::vec3(x+sz, y+sz, z+sz),glm::vec3(sz, sz, sz));
      if(playerBox.rightCollisionTest(neighbour)) return true;
   }
   return false;
}

bool Player::touchingVoxelFrontAt(int x, int y, int z)
{
   const float sz = voxelSize / 2.0f; 
   if(world->is_solid(x, y, z))
   {        
      AABB neighbour(glm::vec3(x+sz, y+sz, z+sz),glm::vec3(sz, sz, sz));
      if(playerBox.frontCollisionTest(neighbour)) return true;
   }
   return false;
}

bool Player::touchingVoxelBackAt(int x, int y, int z)
{
   const float sz = voxelSize / 2.0f; 
   if(world->is_solid(x, y, z))
   {        
      AABB neighbour(glm::vec3(x+sz, y+sz, z+sz),glm::vec3(sz, sz, sz));
      if(playerBox.backCollisionTest(neighbour)) return true;
   }
   return false;
}
   
