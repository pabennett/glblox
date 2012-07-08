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
   worldCentre = glm::vec3(int(dim.x/2), int(dim.y/2), int(dim.z/2));
   useFastMeshBuilder = useFastMeshBuilder;
   int x,y,z;
   vector3i pos;
   vector3i key;
   
   // Build a world of chunks.
   // Ordering z,y,x is important.
   for (z = 0; z < dim.z; z++)
   {
      for (y = 0; y < dim.y; y++)
      {
         for (x = 0; x < dim.x; x++)
         {
            key.x = x;
            key.y = y;
            key.z = z;
            pos.x = x * chunk_size;
            pos.y = y * chunk_size;
            pos.z = z * chunk_size;
            std::cout << "CPP: World: Generating chunk:" << pos.x << "," << pos.y << "," << pos.z << std::endl;
            chunks[key] = new Chunk(pos.x, pos.y, pos.z, chunk_size, dim.y * chunk_size);
         }
      }
   }
}

World::~World()
{
   chunks.clear();
}

// Test to see if the chunk at x,y,z (chunk coords) exists.
bool World::exists(int x, int y, int z)
{
   vector3i key(x,y,z);
   return not (chunks.find(key) == chunks.end());
}

bool World::exists(vector3i key)
{
   return not (chunks.find(key) == chunks.end());
}

// Load a 3D array of bytes into a chunk of equivalent dimensions.
void World::load(byte* data, int x, int y, int z, int size)
{
   // Load the 3D array into the given chunk.
   vector3i key(x,y,z);
   if(exists(key))
   {
      // Load external data into the chunk.
      chunks[key]->load(data, size); 
      std::cout << "CPP: World: Loaded external data into chunk:" 
      << x << "," << y << "," << z << std::endl;
   }
   else
   {
      std::cout << "CPP: Couldnt load this chunk" << std::endl;
   }
   World::chunkUpdateQuery(); // Add modified chunks to the update queue.
}

// Load a 2D heightmap into the world chunks. (world must be pre-sized accordingly)
void World::loadHeightmap(byte* data, int size)
{
   int x,y,z;     // world x,z voxel coords.
   int cy;        // world chunk y coords.
   int h;
   vector3i chunkIndex;
   int vx, vz;
   
   std::vector<byte> heightmap(data, data + ((dim.x*size)*(dim.z*size)));
   std::cout << "CPP: Reading heightmap" << std::endl;
   if(heightmap.size() != (dim.x * chunk_size) * (dim.z * chunk_size))
   {
      std::cout << "CPP: Heightmap dimensions do not match world size!" << std::endl;
      std::cout << "CPP: " << heightmap.size() << std::endl;
      std::cout << "CPP: " << dim.x << "," << dim.y << "," << dim.z << std::endl;
   }
   else
   {
      for(x = 0; x < (dim.x * chunk_size); x++)
      {
         for(z = 0; z < (dim.z * chunk_size); z++)
         {
            vx = (x % chunk_size); // Voxel X
            vz = (z % chunk_size); // Voxel Z
            chunkIndex.x = (x / chunk_size);
            chunkIndex.z = (z / chunk_size);
            h = heightmap[x + (z * chunk_size * dim.x)];
            chunkIndex.y = 0;
            while(h > 0)
            {
               // Create a stack of voxels of height h.
               // setHeight clamps h to the chunk size.
               chunks[chunkIndex]->setHeight(vx,vz,h);
               chunkIndex.y++;
               h-=chunk_size;
            }
         }
      }
      World::chunkUpdateQuery(); // Add modified chunks to the update queue.
   }
}

// The main draw loop call
void World::draw(GLuint program, glm::vec3 camPosition, glm::mat4 mvp)
{
   int x,y,z;
   vertices = 0;
   lastCamPosition = camPosition;
   
   if(!chunks.empty())
   {
      camPositionCheck();
      
      // Pull one chunk from the update queue and update its mesh.
      if(chunkUpdateQueue.size() != 0)
      {
         chunkUpdateQueue.back()->update(true);
         if(!chunkUpdateQueue.back()->meshBuildRunning())
         {
            // If the mesh for this chunk was sucessfully rebuilt remove it from
            // the queue.
            chunkUpdateQueue.pop_back();
            std::cout << "CPP: Chunks awaiting update: " << chunkUpdateQueue.size() << std::endl;
         }
      }

      // Call draw on all chunks to render them.
      for(std::map<vector3i,Chunk*>::iterator i = chunks.begin(); i != chunks.end(); ++i)
      {
         // Render the chunk. Cam is used for culling.
         (*i).second->draw(program, camPosition, mvp, worldViewDistance);
         // Update the vertex counter.
         vertices += (*i).second->verticesRenderedCount;
      }
   }
}

void World::camPositionCheck()
{
   glm::vec3 camChunkPos;
   camChunkPos.x = int(lastCamPosition.x / chunk_size);
   camChunkPos.y = int(lastCamPosition.y / chunk_size);
   camChunkPos.z = int(lastCamPosition.z / chunk_size);
   glm::vec3 chunkPosition;
   int xdel, ydel, zdel;
   glm::vec3 worldBoundMin = glm::vec3(worldCentre.x-(dim.x/2),
                             worldCentre.y-(dim.y/2),
                             worldCentre.z-(dim.z/2));
   glm::vec3 worldBoundMax = glm::vec3(worldCentre.x+(dim.x/2),
                             worldCentre.y+(dim.y/2),
                             worldCentre.z+(dim.z/2));
   
   // Calculate how far away from the original world centre the camera is
   // Measured in chunks.
   xdel = camChunkPos.x - worldCentre.x;
   //ydel = camChunkPos.y - worldCentre.y;
   zdel = camChunkPos.z - worldCentre.z;
   // If the camera has moved in any direction by 1 or more chunks...
   if(xdel != 0 or zdel != 0)
   {
      // Find any chunk lying on the edge of the renderable zone and move it
      // to the opposite edge in the direction of the camera movement.
      // This ensures there are an equal number of chunks rendered in all directions
      // of the camera position.
      for(std::map<vector3i,Chunk*>::iterator i = chunks.begin(); i != chunks.end(); ++i)
      {
         chunkPosition = (*i).second->position();
         if(xdel > 0 and chunkPosition.x == worldBoundMin.x * chunk_size)
         {
            chunkPosition.x = (worldBoundMax.x) * chunk_size;
         }
         else if(xdel < 0 and chunkPosition.x == worldBoundMax.x * chunk_size)
         {
            chunkPosition.x = (worldBoundMin.x) * chunk_size;
         }
         // if(ydel > 0 and chunkPosition.y == worldBoundMin.y * chunk_size)
         // {
            // chunkPosition.y = (worldBoundMax.y) * chunk_size;
         // }
         // else if(ydel < 0 and chunkPosition.y == worldBoundMax.y * chunk_size)
         // {
            // chunkPosition.y = (worldBoundMin.y) * chunk_size;
         // }
         if(zdel > 0 and chunkPosition.z == worldBoundMin.z * chunk_size)
         {
            chunkPosition.z = (worldBoundMax.z) * chunk_size;
         }
         else if(zdel < 0 and chunkPosition.z == worldBoundMax.z * chunk_size)
         {
            chunkPosition.z = (worldBoundMin.z) * chunk_size;
         }
         (*i).second->setChunkPosition( chunkPosition.x, 
                                 chunkPosition.y, 
                                 chunkPosition.z);
      }
      // Update the record of which chunk is currently occupied by the camera.
      worldCentre = camChunkPos;
      std::cout << "CPP: New world centre: " << worldCentre.x << "," << worldCentre.y << "," << worldCentre.z << std::endl;
   }
}
   
   
void World::deleteBlockAt(int x, int y, int z)
{
   int xi, yi, zi;    // Chunk coords (within world)
   int xvi, yvi, zvi; // Voxel coords (within chunk)
   xi = x / chunk_size; 
   yi = y / chunk_size; 
   zi = z / chunk_size; 
   xvi = x % chunk_size;
   yvi = y % chunk_size;
   zvi = z % chunk_size;
   vector3i index(x,y,z);
   // Get the index of the chunk encapsulating the given x,y,z coord.
   if (x >= 0 and xi < dim.x and y >= 0 and yi < dim.y and z >= 0 and zi < dim.z)
   {
      // Delete the voxel of the given coord in this chunk.
      if(!chunks.empty())
      {
         std::cout << "CPP: Testing block at " << xvi << "," << yvi << "," << zvi << std::endl;
         if(chunks[index]->get(xvi, yvi, zvi)>0)
         {
            std::cout << "CPP: Deleting block at " << xvi << "," << yvi << "," << zvi << std::endl;
            chunks[index]->set(xvi, yvi, zvi, 0);
            // Add this chunk to the update queue.
            World::chunkUpdateQuery();
         }
      }
   }
}

// Get the index of the voxel (in chunk space) for the given voxel coord.
// Handles negative and positive coords.
vector3i World::voxelCoordToVoxelIndex(vector3i coord)
{
   int x = (coord.x % chunk_size);
   int y = (coord.y % chunk_size);
   int z = (coord.z % chunk_size);
   
   x = x >= 0 ? x : chunk_size + x;
   y = y >= 0 ? y : chunk_size + y;
   z = z >= 0 ? z : chunk_size + z;

   return vector3i(x,y,z);
}

// For the given voxel obtain the index of the associated chunk in the renderzone.
// Handles negative and positive coords.
vector3i World::voxelCoordToChunkIndex(vector3i coord)
{
   // Convert voxel world coords to chunk world coords.
   int x = coord.x;
   int y = coord.y;
   int z = coord.z;
   // Map chunk world coords to local render space (world) coords.
   x = x >= 0 ? (x / chunk_size) % dim.x : (dim.x - 1) + (((x + 1) / chunk_size) % dim.x);
   y = y >= 0 ? (y / chunk_size) % dim.y : (dim.y - 1) + (((y + 1) / chunk_size) % dim.y);
   z = z >= 0 ? (z / chunk_size) % dim.z : (dim.z - 1) + (((z + 1) / chunk_size) % dim.z);
      
   return vector3i(x,y,z);
}

// For the given chunk coord (world space) obtain the associated
// chunk index for the render zone.
vector3i World::chunkCoordToChunkIndex(vector3i coord)
{
   int x = coord.x % dim.x;
   int y = coord.y % dim.y;
   int z = coord.z % dim.z;
   
   x = x >= 0? x % dim.x : dim.x + (x % dim.x);
   x = y >= 0? y % dim.y : dim.y + (y % dim.y);   
   x = z >= 0? z % dim.z : dim.z + (z % dim.z);   
}

/* Delete a spherical region of voxels about the world x,y,z co-ordinates
   with the specified radius. 
   Operates across multiple chunks if necessary
   The input co-ordinates are true voxel x,y,z co-ordinates, we must find out
   which chunk currently holds the voxels at this co-ordinate and operate on
   that chunk only. */

void World::modifyRegionAt(int x, int y, int z, byte val, int r)
{
   int xi, yi, zi;      // Voxel coords (within world)
   
   vector3i chunkIndex; // Index of the chunk in the render space.
   vector3i voxelIndex; // Index of the voxel in the chunk.
   
   int r_sq = r * r;  
   int x_start = x - r;
   int y_start = y - r;
   int z_start = z - r;
   int x_end = x + r;
   int y_end = y + r;
   int z_end = z + r;

   if(!chunks.empty())
   {
      // For all voxels in the cubic region bounding the sphere radius r:
      for (xi = x_start; xi < x_end; xi++)
      {
         for (yi = y_start; yi < y_end; yi++)
         {
            for (zi = z_start; zi < z_end; zi++)
            {       
               // Test to see if xi,yi,zi lies within the sphere.
               if (((xi-x)*(xi-x) + (yi-y)*(yi-y) + (zi-z)*(zi-z)) < r_sq)
               {
                  // Get the indices for the voxel and its chunk.
                  chunkIndex = voxelCoordToChunkIndex(vector3i(xi,yi,zi));
                  voxelIndex = voxelCoordToVoxelIndex(vector3i(xi,yi,zi));
                  // Delete the voxel.
                  if(exists(chunkIndex))
                  {
                     chunks[chunkIndex]->set(voxelIndex.x,voxelIndex.y,voxelIndex.z,val);
                  }
               }
            }
         }
      }
      World::chunkUpdateQuery(); // Add modified chunks to the update queue.
   }
}

void World::chunkUpdateQuery()
{
   for(std::map<vector3i,Chunk*>::iterator i = chunks.begin(); i != chunks.end(); ++i)
   {
      // Add all stale chunks to the chunk update queue.
      if ((*i).second->requireMeshUpdate())
      {
         (*i).second->initialiseMeshBuilder();
         chunkUpdateQueue.push_back((*i).second);
      }
   }
}

void World::fillSpheres()
{
   // Create a volume where each chunk contains a sphere.
   int radius = chunk_size/2;
   int r_sq = radius * radius;
   int h = radius;
   int j = radius;
   int k = radius;
   int x,y,z;
   vector3i key;
   
   if(!chunks.empty())
   {
      for(std::map<vector3i,Chunk*>::iterator i = chunks.begin(); i != chunks.end(); ++i)
      {
         std::cout << "CPP: Filling chunk with sphere data..." << std::endl;
         int64 time = GetTimeMs64();
         (*i).second->empty();
         for (x = 0; x < chunk_size; x++)
         {
            for (y = 0; y < chunk_size; y++)
            {
               for (z = 0; z < chunk_size; z++)
               {
                  if (((x-h)*(x-h) + (y-j)*(y-j) + (z-k)*(z-k)) < r_sq)
                  {
                     key.x = x;
                     key.y = y;
                     key.z = z;
                     (*i).second->set(x,y,z,1);
                  }
               }
            }
         }
         std::cout << "CPP: ... fill complete (" << GetTimeMs64() - time << "ms)" << std::endl;
      }
      World::chunkUpdateQuery(); // Add modified chunks to the update queue.
   }
}

void World::fillPyramids()
{
   // Create a volume where each chunk contains a pyramid.
   int x,y,z = 0;
   if(!chunks.empty())
   {
      for(std::map<vector3i,Chunk*>::iterator i = chunks.begin(); i != chunks.end(); ++i)
      {
         std::cout << "CPP: Filling chunk with pyramid data..." << std::endl;
         int64 time = GetTimeMs64();
         (*i).second->empty();
         for (x = 0; x < chunk_size; x++)
         {
            for (y = 0; y < chunk_size; y++)
            {
               for (z = 0; z < chunk_size; z++)
               {
                  if (x >= y and x <= chunk_size-y and z >= y and z <= chunk_size-y)
                  {
                     (*i).second->set(x,y,z,1);
                  }
               }
            }
         }
         std::cout << "CPP: ... fill complete (" << GetTimeMs64() - time << "ms)" << std::endl;
      }
      World::chunkUpdateQuery(); // Add modified chunks to the update queue.
   }
}


void World::fill()
{
   int x,y,z;
   if(!chunks.empty())
   {
      for(std::map<vector3i,Chunk*>::iterator i = chunks.begin(); i != chunks.end(); ++i)
      {
         (*i).second->fill();
      }
      World::chunkUpdateQuery(); // Add modified chunks to the update queue.
   }
}

void World::setViewDistance(int distance)
{
   worldViewDistance = distance;
}

void World::random()
{
   int x,y,z;        // Voxel indices
   int cx, cy, cz;   // Chunk indices
   int h;
   int size_sq = dim.x * dim.y;
   
   float simplex;
   if(!chunks.empty())
   {
      std::cout << "CPP: Randomising chunks using simplex noise function..." << std::endl;
      for(std::map<vector3i,Chunk*>::iterator i = chunks.begin(); i != chunks.end(); ++i)
      {
         cz = (*i).first.z * chunk_size;
         cy = (*i).first.y * chunk_size;
         cx = (*i).first.x * chunk_size;
                  
         
         (*i).second->empty();
         for (x = 0; x < chunk_size; x++)
         {
            for (z = 0; z < chunk_size; z++)
            {
               //for (y = 0; y < chunk_size; y++)
               //{
                  simplex = glm::simplex(glm::vec2((cx+x)/256.f,(cz+z)/256.f));
                  h = int(simplex * float(chunk_size));
                  (*i).second->setHeight(x,z,h);
               //}
            }
         }
      }
      World::chunkUpdateQuery(); // Add modified chunks to the update queue.
   }
}

// Load or generate data for the chunk at coords x,y,z.
// x,y,z are chunk coords in world space.
//void World::loadChunk(int x, int y, int z)
//{
//   vector3i chunkIndex = 