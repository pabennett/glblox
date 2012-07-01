////////////////////////////////////////////////////////////////////////////////
// GlBlox : A library for rendering voxel volumes as a triangular mesh.
// 
// @author         Peter A Bennett
// @copyright      (c) 2012 Peter A Bennett
// @license        LGPL      
// @email          pab850@googlemail.com
// @contact        www.bytebash.com
//
////////////////////////////////////////////////////////////////////////////////

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>    // OpenGL Extension Wrangler
#include <GL/wglew.h>

#include <iostream>
#include "types.hpp"    // Collection of common types.
#include "world.cpp"    // A class describing a collection of chunks.
                        // Provides high level access to the chunk data.
#include "camera.cpp"   // Provides a quaternion based first person camera ,
                        
extern "C" {

   void initGLEW()
   {
      GLenum err = glewInit();
      if (GLEW_OK != err)
      {
         // glewInit failed, something is seriously wrong
         std::cout << "Glew is buggered mate" << std::endl;
      }
      std::cout << "CPP: GLEW Initialised." << std::endl;
   }
   
   // Camera
   Camera* newCamera(int width, 
                     int height, 
                     float vertFOV, 
                     float frustrumNear, 
                     float frustrumFar,
                     bool flightMode)
   {
      return new Camera(width, 
                        height, 
                        vertFOV, 
                        frustrumNear, 
                        frustrumFar, 
                        flightMode);
   }
   
   void C_setMVP(Camera* obj, GLuint location) { obj->setMVP(location);}
   
   void C_setPerspective(  Camera* obj, 
                           int w, 
                           int h, 
                           float fov, 
                           float fzN, 
                           float fzF) 
   { 
      obj -> perspective(w, h, fov, fzN, fzF);
   }
   void C_setPos(Camera* obj, float x, float y, float z) 
   {
      obj -> setPos(x, y, z);
   }
   float C_getPosX(Camera* obj) 
   {
      return obj -> getPosX();
   }
   float C_getPosY(Camera* obj) 
   {
      return obj -> getPosY();
   }
   float C_getPosZ(Camera* obj) 
   {
      return obj -> getPosZ();
   }
   void C_Orient(Camera* obj, float yaw, float pitch) 
   {
      obj -> orient(yaw, pitch);
   }
   void C_Move(Camera* obj, float dx, float dy, float dz) 
   {
      obj -> move(dx, dy, dz);
   }

   World* newWorld(int dimx, 
                   int dimy, 
                   int dimz, 
                   int chunk_size, 
                   bool useFastMeshBuilder)
   {
      return new World(dimx, dimy, dimz, chunk_size, useFastMeshBuilder);
   }
   
   void worldDraw(World* world, Camera* camera, GLuint program)
   {
      world->draw(program, camera->position, camera->mvp);
   }
   
   void worldFillSpheres(World* world)
   {
      world->fillSpheres();
   }
   
   void worldFill(World* world)
   {
      world->fill();
   }
   
   void worldRandom(World* world)
   {
      world->random();
   }
   
   void worldSetViewDistance(World* world, int distance)
   {
      world->setViewDistance(distance);
   }
      
   void worldDeleteBlockAt(World* world, int x, int y, int z)
   {
      world->deleteBlockAt(x, y, z);
   }
   
   void worldModifyRegionAt(World* world, int x, int y, int z, byte val, int radius)
   {
      world->modifyRegionAt(x, y, z, val, radius);
   }
   
   int worldNumVertices(World* world)
   {
      return world->vertices;
   }
   void worldLoad(World* world, byte* data, int x, int y, int z, int chunk_size)
   {
      world->load(data, x, y, z, chunk_size);
   }
}