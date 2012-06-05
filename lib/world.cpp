////////////////////////////////////////////////////////////////////////////////
// World          A wrapper class for the Chunk class. The World class stores
//                many Chunks in order to represent a larger Volume or 'World'.
//                Chunks are used to subdivide a 'World' volume as it allows
//                quicker modification, access and rendering
//
// @author         Peter A Bennett
// @copyright      (c) 2012 Peter A Bennett
// @version        $Rev: 2 $
// @lastrevision   $Date: 2012-03-11 15:19:25 +0000 (Sun, 11 Mar 2012) $
// @license        LGPL      
// @email          pab850@googlemail.com
// @contact        www.bytebash.com
//
////////////////////////////////////////////////////////////////////////////////

#include "world.hpp"
                                             
World::World(int dimx, int dimy, int dimz, int size, bool useFastMeshBuilder)
{
   // Chunk size must be power of 2
   if(!((size > 0) && ((size & (size - 1)) == 0)))
   {
      return;
   }
   chunk_size = size;
   dim.x = dimx;
   dim.y = dimy;
   dim.z = dimz;
   World::useFastMeshBuilder = useFastMeshBuilder;
   vector3i pos;
   int x,y,z;
   
   // Build a world of chunks.
   // Ordering z,y,x is important.
   for (z = 0; z < dim.z; z++)
   {
      for (y = 0; y < dim.y; y++)
      {
         for (x = 0; x < dim.x; x++)
         {
            pos.x = x * chunk_size;
            pos.y = y * chunk_size;
            pos.z = z * chunk_size;
            std::cout << "CPP: World: Generating chunk:" << pos.x << "," << pos.y << "," << pos.z << std::endl;
            chunks.push_back(new Chunk(pos.x, pos.y, pos.z, chunk_size));
         }
      }
   }
}

World::~World()
{
   chunks.clear();
}

void World::load(byte* data, int x, int y, int z, int size)
{
   // Load the 3D array into the given chunk.
   int index = x + y * dim.x + z * dim.x * dim.y;
   if(!chunks.empty() and chunks.size() >= index)
   {
      // Load external data into the chunk.
      chunks[index]->load(data, size); 
      std::cout << "CPP: World: Loaded external data into chunk:" 
      << x << "," << y << "," << z << std::endl;
   }
}
   

void World::draw(GLuint program, glm::vec3 camPosition, glm::mat4 mvp)
{
   int x,y,z;
   vertices = 0;
   if(!chunks.empty())
   {
      for(std::vector<Chunk*>::size_type i = 0; i != chunks.size(); i++)
      {
         // Render the chunk. Cam is used for culling.
         chunks[i]->draw(program, camPosition, mvp, useFastMeshBuilder);
         // Update the vertex counter.
         vertices += chunks[i]->verticesRenderedCount;
      }
   }
}

void World::fillSpheres()
{
   int x,y,z;
   if(!chunks.empty())
   {
      for(std::vector<Chunk*>::size_type i = 0; i != chunks.size(); i++)
      {
         std::cout << "CPP: Filling chunk with sphere data..." << std::endl;
         int64 time = GetTimeMs64();
         chunks[i]->sphere(); // Create a sphere in the chunk.
         std::cout << "CPP: ... fill complete (" << GetTimeMs64() - time << "ms)" << std::endl;
      }
   }
}

void World::fill()
{
   int x,y,z;
   if(!chunks.empty())
   {
      for(std::vector<Chunk*>::size_type i = 0; i != chunks.size(); i++)
      {
         chunks[i]->fill(); // Create a sphere in the chunk.
      }
   }
}

void World::random()
{
   int x,y,z;
   if(!chunks.empty())
   {
      std::cout << "CPP: Randomising chunks using simplex noise function..." << std::endl;
      for(std::vector<Chunk*>::size_type i = 0; i != chunks.size(); i++)
      {
         chunks[i]->random();
      }
   }
}