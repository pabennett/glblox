// OpenGL Mathematics Library
#include <glm/glm.hpp>
#include <vector>
#include "chunk.cpp"    // Defines a chunk class which acts as a voxel container
                        // and provides utility functions for accessing or 
                        // modifying the data as well as rendering the data. 
                        

           
class World
{
      std::map<vector3i,Chunk*> chunks;
      vector3i dim;                                // The dimensions of the world in chunks.
      int chunk_size;                              // The dimension of the world chunks.
      std::vector<Chunk*> chunks2;                 // Array of chunks representing the world.
      std::vector<Chunk*> chunkUpdateQueue;        // Chunks that require a mesh update.
      std::vector<Chunk*> chunkOptimiseQueue;      // The queue of chunks that require mesh optimisation.
   public:
      int vertices;                                // The number of vertices required to draw the world.
      World(int, int, int, int, bool);             // Construct
      ~World();                                    // Destruct
      void draw(GLuint, glm::vec3, glm::mat4);     // Draw the world.
      void fillSpheres();                          // Fill the world with spheres.
      void fillPyramids();                         // Fill the world with pyramids.
      void fill();                                 // Fill the world.
      void random();                               // Randomly generated world.
      bool exists(int, int, int);                  // Test to see if the chunk at the chunk x,y,z coords exists.
      bool exists(vector3i);
      void deleteBlockAt(int, int, int);           // Delete the voxel at the specified world coord.
      void modifyRegionAt(int, int, int, byte, int);// Delete a spherical region of voxels at the
                                                   // specified world coord with the specified radius.
      void load(byte*, int, int, int, int);
      void setViewDistance(int);                   // Set the viewing distance.
   private:
      vector3i voxelCoordToVoxelIndex(vector3i); // Translate world voxel coords to voxel indices (chunk space).
      vector3i voxelCoordToChunkIndex(vector3i); // Translate world voxel coords to chunk indices (world 'renderable zone' space).
      bool useFastMeshBuilder;                     // Use fast mesh generation.
      void chunkUpdateQuery();                     // Check all chunks to see which ones need an update.
      int worldViewDistance;                       // How far you can see measured in voxels.
      glm::vec3 lastCamPosition;                   // The last known position of the camera.
      void camPositionCheck();                     // If the camera has moved into a new chunk we may need to draw a new part of the world.
      glm::vec3 worldCentre;                       // The chunk that is central in the world.
};