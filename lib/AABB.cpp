////////////////////////////////////////////////////////////////////////////////
// AABB            A class for managing AABB collision detection.
//
// 
// @author         Peter A Bennett
// @copyright      (c) 2012 Peter A Bennett
// @license        LGPL      
// @email          pab850@googlemail.com
// @contact        www.bytebash.com
//
////////////////////////////////////////////////////////////////////////////////

#include "AABB.hpp"

// To deal with floating point comparison problems.
// Probably a better solution! TODO:
float epsilon = 1e-2f; 

AABB::AABB()
{
}

AABB::AABB(glm::vec3 centre, glm::vec3 extents)
{              
   AABB::centre = centre;
   AABB::extents = extents;
   AABB::max = maxVector();
   AABB::min = minVector();
}

void AABB::setPosition(glm::vec3 centre)
{
   AABB::centre = centre;
   AABB::max = maxVector();
   AABB::min = minVector();
}

collisionStatus AABB::collisionTest(AABB other)
{
   collisionStatus result;
   
   result.left = leftCollisionTest(other);
   result.right = rightCollisionTest(other);
   result.back = backCollisionTest(other);
   result.front = frontCollisionTest(other);
   result.bottom = bottomCollisionTest(other);
   result.top = topCollisionTest(other);
   
   return result;
}

// Check this AABB against the other AABB and indicate if there is
// a collision. Indicate which face of this AABB is in contact with the other.
bool AABB::leftCollisionTest(AABB other)
{
   /*                               +-------+
      Left face collision scenarios |       |
                                    | self  |
      +-------+-------+     +-------|       |      
      |       |       |     |       +-------+          
      | other | self  |     | other |          
      |       |       |     |       +-------+  
      +-------+-------+     +-------|       |     
              |                     | self  |
       other.xmax <= self.xmin      |       |
                                    +-------+
                                  /
                       self.zmin < other.zmin, but self.zmax >= other.zmin.
                       
   */
   if(max.z - epsilon > other.min.z and min.z + epsilon < other.max.z and
      max.y - epsilon > other.min.y and min.y + epsilon < other.max.y and
      min.x <= other.max.x and max.x >= other.max.x)
   {
      return true;
   }
   return false;
}

bool AABB::rightCollisionTest(AABB other)
{   
   if(max.z - epsilon > other.min.z and min.z + epsilon < other.max.z and
      max.y - epsilon > other.min.y and min.y + epsilon < other.max.y and
      max.x >= other.min.x and min.x <= other.max.x)
   {
      return true;
   }
   return false;
}

bool AABB::backCollisionTest(AABB other)
{
   if(max.x - epsilon > other.min.x and min.x + epsilon < other.max.x and
      max.y - epsilon > other.min.y and min.y + epsilon < other.max.y and
      min.z <= other.max.z and max.z >= other.max.z)
   {
      return true;
   }
   return false;
}

bool AABB::frontCollisionTest(AABB other)
{   
   if(max.x - epsilon > other.min.x and min.x + epsilon < other.max.x and
      max.y - epsilon > other.min.y and min.y + epsilon < other.max.y and
      max.z >= other.min.z and min.z <= other.max.z)
   {
      return true;
   }
   return false;
}

bool AABB::bottomCollisionTest(AABB other)
{
   if(max.x - epsilon > other.min.x and min.x + epsilon < other.max.x and
      max.z - epsilon > other.min.z and min.z + epsilon < other.max.z and
      min.y <= other.max.y and max.y >= other.max.y)
   {
      return true;
   }
   return false;
}

bool AABB::topCollisionTest(AABB other)
{   
   if(max.x - epsilon > other.min.x and min.x + epsilon < other.max.x and
      max.z - epsilon > other.min.z and min.z + epsilon < other.max.z and
      max.y >= other.min.y and min.y <= other.max.y)
   {
      return true;
   }
   return false;
}

// Return true if AABB other contains AABB self
bool AABB::contains(AABB other)
{
   if(other.min.x >= min.x and other.max.x <= max.x and
      other.min.y >= min.y and other.max.y <= max.y and
      other.min.z >= min.z and other.max.z <= max.z)
   {
      return true;
   }
   return false;
}

glm::vec3 AABB::minVector()
{
   return glm::vec3( centre.x - extents.x,
                     centre.y - extents.y,
                     centre.z - extents.z);
}

glm::vec3 AABB::maxVector()
{
   return glm::vec3( centre.x + extents.x,
                     centre.y + extents.y,
                     centre.z + extents.z);
}


   