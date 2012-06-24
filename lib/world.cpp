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
            chunks.push_back(new Chunk(pos.x, pos.y, pos.z, chunk_size, dim.y * chunk_size));
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
   World::chunkUpdateQuery(); // Add modified chunks to the update queue.
}
   

void World::draw(GLuint program, glm::vec3 camPosition, glm::mat4 mvp)
{
   int x,y,z;
   vertices = 0;
   if(!chunks.empty())
   {
      // Pull one chunk from the update queue and update its mesh.
      if(chunkUpdateQueue.size() != 0)
      {
         chunkUpdateQueue.back()->update(useFastMeshBuilder);
         chunkUpdateQueue.pop_back();
      }
      // Call draw on all chunks to render them.
      for(std::vector<Chunk*>::size_type i = 0; i != chunks.size(); i++)
      {
         // Render the chunk. Cam is used for culling.
         chunks[i]->draw(program, camPosition, mvp, useFastMeshBuilder);
         // Update the vertex counter.
         vertices += chunks[i]->verticesRenderedCount;
      }
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
   int index = xi + yi * dim.x + zi * dim.x * dim.y; // Index of chunk
   // Get the index of the chunk encapsulating the given x,y,z coord.
   if (x >= 0 and xi < dim.x and y >= 0 and yi < dim.y and z >= 0 and zi < dim.z)
   {
      // Delete the voxel of the given coord in this chunk.
      if(!chunks.empty() and chunks.size() >= index)
      {
         std::cout << "CPP: Testing block at " << xvi << "," << yvi << "," << zvi << std::endl;
         if(chunks[index]->get(xvi, yvi, zvi)>0)
         {
            std::cout << "CPP: Deleting block at " << xvi << "," << yvi << "," << zvi << std::endl;
            chunks[index]->set(xvi, yvi, zvi, 0);
            // Add this chunk to the update queue.
            chunkUpdateQueue.push_back(chunks[index]);
         }
      }
   }
}

/* Delete a spherical region of voxels about the world x,y,z co-ordinates
   with the specified radius. 
   Operates across multiple chunks if necessary.*/

void World::modifyRegionAt(int x, int y, int z, byte val, int r)
{
   int xvi, yvi, zvi; // Voxel coords (within chunk)
   int xi, yi, zi;    // Voxel coords (within world)
   int chunk_index;
   
   int r_sq = r * r;

   int x_start = x - r >= 0 ? x - r : 0;
   int y_start = y - r >= 0 ? y - r : 0;
   int z_start = z - r >= 0 ? z - r : 0;
   int x_end = x + r < (dim.x * chunk_size) ? x + r : dim.x * chunk_size;
   int y_end = y + r < (dim.y * chunk_size) ? y + r : dim.y * chunk_size;
   int z_end = z + r < (dim.z * chunk_size) ? z + r : dim.z * chunk_size;

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
                  // Determine the index of the chunk holding xi,yi,zi
                  chunk_index =  (xi / chunk_size) + 
                                 (yi / chunk_size) * dim.x +
                                 (zi / chunk_size) * dim.x * dim.y;
                    
                  // Determine the voxel coords (chunk space) of xi,yi,zi
                  xvi = xi % chunk_size;
                  yvi = yi % chunk_size;
                  zvi = zi % chunk_size;  
                  // Delete xi,yi,zi
                  chunks[chunk_index]->set(xvi,yvi,zvi,val);
               }
            }
         }
      }
      World::chunkUpdateQuery(); // Add modified chunks to the update queue.
   }
}

void World::chunkUpdateQuery()
{
   for(std::vector<Chunk*>::size_type i = 0; i != chunks.size(); i++)
   {
      // Add all stale chunks to the chunk update queue.
      if (chunks[i]->is_modified())
      {
         chunkUpdateQueue.push_back(chunks[i]);
         chunks[i]->clearModifiedState();
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
   
   if(!chunks.empty())
   {
      for(std::vector<Chunk*>::size_type i = 0; i != chunks.size(); i++)
      {
         std::cout << "CPP: Filling chunk with sphere data..." << std::endl;
         int64 time = GetTimeMs64();
         chunks[i]->empty();
         for (x = 0; x < chunk_size; x++)
         {
            for (y = 0; y < chunk_size; y++)
            {
               for (z = 0; z < chunk_size; z++)
               {
                  if (((x-h)*(x-h) + (y-j)*(y-j) + (z-k)*(z-k)) < r_sq)
                  {
                     chunks[i]->set(x,y,z,1);
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
      for(std::vector<Chunk*>::size_type i = 0; i != chunks.size(); i++)
      {
         std::cout << "CPP: Filling chunk with pyramid data..." << std::endl;
         int64 time = GetTimeMs64();
         chunks[i]->empty();
         for (x = 0; x < chunk_size; x++)
         {
            for (y = 0; y < chunk_size; y++)
            {
               for (z = 0; z < chunk_size; z++)
               {
                  if (x >= y and x <= chunk_size-y and z >= y and z <= chunk_size-y)
                  {
                     chunks[i]->set(x,y,z,1);
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
      for(std::vector<Chunk*>::size_type i = 0; i != chunks.size(); i++)
      {
         chunks[i]->fill();
      }
      World::chunkUpdateQuery(); // Add modified chunks to the update queue.
   }
}

void World::random()
{
   int x,y,z;        // Voxel indices
   int cx, cy, cz;   // Chunk indices
      
   int size_sq = dim.x * dim.y;
   
   float simplex;
   if(!chunks.empty())
   {
      std::cout << "CPP: Randomising chunks using simplex noise function..." << std::endl;
      for(std::vector<Chunk*>::size_type i = 0; i != chunks.size(); i++)
      {
         cz = (i / size_sq) * chunk_size;
         cy = ((i % size_sq) / dim.x) * chunk_size;
         cx = (i % dim.x) * chunk_size;
         
         chunks[i]->empty();
         for (x = 0; x < chunk_size; x++)
         {
            for (z = 0; z < chunk_size; z++)
            {
               for (y = 0; y < chunk_size; y++)
               {
                  simplex = glm::simplex(glm::vec3((cx+x) / 32.f, (cy+y) / 64.f, (cz+z) / 32.f)) * 255;
                  simplex = simplex >0.5f?1:0;
                  chunks[i]->set(x,y,z,int(simplex));
               }
            }
         }
      }
      World::chunkUpdateQuery(); // Add modified chunks to the update queue.
   }
}