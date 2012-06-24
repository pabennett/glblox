// OpenGL Mathematics Library
#include <glm/glm.hpp>
#include <vector>
#include "chunk.cpp"    // Defines a chunk class which acts as a voxel container
                        // and provides utility functions for accessing or 
                        // modifying the data as well as rendering the data. 
           
class World
{
      vector3i dim;                                // The dimensions of the world in chunks.
      int chunk_size;                              // The dimension of the world chunks.
      std::vector<Chunk*> chunks;                  // Array of chunks representing the world.
      std::vector<Chunk*> chunkUpdateQueue;        // Chunks that require a mesh update.
   public:
      int vertices;                                // The number of vertices required to draw the world.
      World(int, int, int, int, bool);             // Construct
      ~World();                                    // Destruct
      void draw(GLuint, glm::vec3, glm::mat4);     // Draw the world.
      void fillSpheres();                          // Fill the world with spheres.
      void fillPyramids();                         // Fill the world with pyramids.
      void fill();                                 // Fill the world.
      void random();                               // Randomly generated world.
      void deleteBlockAt(int, int, int);           // Delete the voxel at the specified world coord.
      void deleteRegionAt(int, int, int, int);     // Delete a spherical region of voxels at the
                                                   // specified world coord with the specified radius.
      void load(byte*, int, int, int, int);
   private:
      bool useFastMeshBuilder;                     // Use fast mesh generation.
      void chunkUpdateQuery();                     // Check all chunks to see which ones need an update.
};