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
#include "player.cpp"   // Provides a player character.
                        
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
   
   // Player
   Player* newPlayer(Camera* camera,
                     World* world)
   {
      return new Player(camera, world);
   }
   
   void playerMove(Player* obj,
                   float dx,
                   float dy,
                   float dz)
   {
      obj->move(dx, dy, dz);
   }
   
   void playerUpdate(Player* obj,
               float dt,
               bool movingForward,
               bool movingBackward,
               bool movingLeft,
               bool movingRight)
   {
      obj->update(dt, movingForward, movingBackward, movingLeft, movingRight);
   }
   
   void playerOrient(Player* obj,
                     float pitch,
                     float yaw)
   {
      obj->orient(pitch, yaw);
   }
   
   void playerSetCamMVP(Player* obj)
   {
      obj->setCameraMVP();
   }
   
   float playerGetPosX(Player* obj)
   {
      return obj->getPositionX();
   }
   
   float playerGetPosY(Player* obj)
   {
      return obj->getPositionY();
   }
   
   float playerGetPosZ(Player* obj)
   {
      return obj->getPositionZ();
   }
      
   float playerGetVelocityX(Player* obj)
   {
      return obj->getPlayerVelocityX();
   }
   
   float playerGetVelocityY(Player* obj)
   {
      return obj->getPlayerVelocityY();
   }
   
   float playerGetVelocityZ(Player* obj)
   {
      return obj->getPlayerVelocityZ();
   }
      
   // Camera
   Camera* newCamera(int width, 
                     int height, 
                     float vertFOV, 
                     float frustrumNear, 
                     float frustrumFar,
                     bool flightMode,
                     GLuint program)
   {
      return new Camera(width, 
                        height, 
                        vertFOV, 
                        frustrumNear, 
                        frustrumFar, 
                        flightMode,
                        program);
   }
   
   void C_setMVP(Camera* obj) { obj->setMVP();}
   
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
                   bool xWrap,
                   bool yWrap,
                   bool zWrap,
                   GLuint program)
   {
      return new World(dimx, dimy, dimz, 
                       chunk_size, 
                       xWrap, yWrap, zWrap,
                       program);
   }
   
   void worldDraw(World* world, Player* player)
   {
      world->draw(player->getPosition(), player->getPlayerCamMVP());
   }
   
   void worldFillSpheres(World* world)
   {
      world->fillSpheres();
   }
   
   void worldFill(World* world)
   {
      world->fill();
   }
   
   void worldSetRandomTerrainEnabledState(World* world, bool enabled)
   {
      world->setRandomTerrainEnabledState(enabled);
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
   
   int worldChunksAwaitingUpdate(World* world)
   {
      return world->chunksAwaitingUpdate();
   }
   
   void worldModifyRegionAt(World* world, int x, int y, int z, byte val, int radius)
   {
      world->modifyRegionAt(x, y, z, val, radius);
   }
   
   int worldNumVertices(World* world)
   {
      return world->getNumVertices();
   }
   void worldLoad(World* world, byte* data, int x, int y, int z, int chunk_size)
   {
      world->load(data, x, y, z, chunk_size);
   }
   void worldLoadHeightmap(World* world, byte* data, int chunk_size)
   {
      world->loadHeightmap(data, chunk_size);
   }
}