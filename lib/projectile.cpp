////////////////////////////////////////////////////////////////////////////////
// Projectile      A class describing a projectile.
//
// 
// @author         Peter A Bennett
// @copyright      (c) 2012 Peter A Bennett
// @license        LGPL      
// @email          pab850@googlemail.com
// @contact        www.bytebash.com
//
////////////////////////////////////////////////////////////////////////////////

#include "projectile.hpp"

Projectile::Projectile(World* world, glm::vec3 position, glm::vec3 velocity, GLuint program) 
{              
   Projectile::position = position;
   Projectile::velocity = velocity;
   Projectile::worldPtr = world;
   Projectile::program = program;
   Projectile::hitbox = AABB(position, glm::vec3(0.5f, 0.5f, 0.5f));
   Projectile::age = 0.0f;
   
   glGenBuffers(1, &VBO);
   
	glUseProgram(program);
   worldPosAttrib = glGetUniformLocation(program, "worldPosition");
   posAttrib = glGetAttribLocation(program, "position");
	glUseProgram(0);
   
   vertex v;
   
   // Above face
   v.x = 1; v.y = 1; v.z = 1; vertices.push_back(v);
   v.x = 1; v.y = 1; v.z = 0; vertices.push_back(v);
   v.x = 0; v.y = 1; v.z = 1; vertices.push_back(v);
   v.x = 0; v.y = 1; v.z = 1; vertices.push_back(v);
   v.x = 1; v.y = 1; v.z = 0; vertices.push_back(v);
   v.x = 0; v.y = 1; v.z = 0; vertices.push_back(v);
   // Below Face
   v.x = 1; v.y = 0; v.z = 1; vertices.push_back(v);
   v.x = 0; v.y = 0; v.z = 1; vertices.push_back(v);
   v.x = 1; v.y = 0; v.z = 0; vertices.push_back(v);
   v.x = 1; v.y = 0; v.z = 0; vertices.push_back(v);
   v.x = 0; v.y = 0; v.z = 1; vertices.push_back(v);
   v.x = 0; v.y = 0; v.z = 0; vertices.push_back(v);
   // Left face
   v.x = 0; v.y = 1; v.z = 1; vertices.push_back(v);
   v.x = 0; v.y = 1; v.z = 0; vertices.push_back(v);
   v.x = 0; v.y = 0; v.z = 1; vertices.push_back(v);
   v.x = 0; v.y = 0; v.z = 1; vertices.push_back(v);
   v.x = 0; v.y = 1; v.z = 0; vertices.push_back(v);
   v.x = 0; v.y = 0; v.z = 0; vertices.push_back(v);
   // Right face
   v.x = 1; v.y = 1; v.z = 1; vertices.push_back(v);
   v.x = 1; v.y = 0; v.z = 1; vertices.push_back(v);
   v.x = 1; v.y = 1; v.z = 0; vertices.push_back(v);
   v.x = 1; v.y = 1; v.z = 0; vertices.push_back(v);
   v.x = 1; v.y = 0; v.z = 1; vertices.push_back(v);
   v.x = 1; v.y = 0; v.z = 0; vertices.push_back(v);
   // Front face
   v.x = 1; v.y = 1; v.z = 1; vertices.push_back(v);
   v.x = 0; v.y = 1; v.z = 1; vertices.push_back(v);
   v.x = 0; v.y = 0; v.z = 1; vertices.push_back(v);
   v.x = 0; v.y = 0; v.z = 1; vertices.push_back(v);
   v.x = 1; v.y = 0; v.z = 1; vertices.push_back(v);
   v.x = 1; v.y = 1; v.z = 1; vertices.push_back(v);
   // Back face
   v.x = 0; v.y = 0; v.z = 0; vertices.push_back(v);
   v.x = 0; v.y = 1; v.z = 0; vertices.push_back(v);
   v.x = 1; v.y = 1; v.z = 0; vertices.push_back(v);
   v.x = 1; v.y = 1; v.z = 0; vertices.push_back(v);
   v.x = 1; v.y = 0; v.z = 0; vertices.push_back(v);
   v.x = 0; v.y = 0; v.z = 0; vertices.push_back(v);
   
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Projectile::~Projectile()
{
   vertices.clear();   
   worldPtr->modifyRegionAt(position.x, position.y, position.z, 0, 5);
}

void Projectile::update(float dt)
{
   float movementSpeed = dt * 80.0f;
   float gravity = 120.0f;
   float damping = 1.0f;
   glm::vec3 surfaceNormal;
   age += dt;
   
   // Collision test
   if(worldPtr->is_solid(position.x, position.y, position.z) or age > 1.0f)
   {
      velocity = glm::vec3(0, 0, 0);
   }
   
   velocity -= velocity * dt * damping; 
   position += velocity * dt;
}

bool Projectile::collisionTest()
{
   // TODO
}
   

bool Projectile::stopped()
{
   if(glm::length(velocity) < 4.0f)
   {
      return true;
   }
   return false;
}

void Projectile::draw()
{
   glUseProgram(program);
   glUniform3fv(worldPosAttrib, 1, glm::value_ptr(position));
   glEnableVertexAttribArray(posAttrib);  
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glVertexAttribPointer(posAttrib, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
   glDrawArrays(GL_TRIANGLES, 0, vertices.size());
   glDisableVertexAttribArray(posAttrib);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glUseProgram(0);
}