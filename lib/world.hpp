// OpenGL Mathematics Library
#include <glm/glm.hpp>
#include <vector>
#include "chunk.cpp"    // Defines a chunk class which acts as a voxel container
                        // and provides utility functions for accessing or 
                        // modifying the data as well as rendering the data. 
#include "perlin.cpp"
                        

           
class World
{
      std::map<vector3i,Chunk*> chunks;
      vector3i dim;                                // The dimensions of the world in chunks.
      int chunk_size;                              // The dimension of the world chunks.
      std::vector<Chunk*> chunks2;                 // Array of chunks representing the world.
      std::vector<Chunk*> chunkUpdateQueue;        // Chunks that require a mesh update.
      std::vector<Chunk*> chunkOptimiseQueue;      // The queue of chunks that require mesh optimisation.
      std::vector<Chunk*> region2DLoaderQueue;     // Queue for loading 2D regions.
   public:
      int vertices;                                // The number of vertices required to draw the world.
      World(int, int, int, int, bool, bool, bool); // Construct
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
      void loadHeightmap(byte*, int);
      void setViewDistance(int);                   // Set the viewing distance.
      void loadRegion(int, int);                   // Load data for the given x,z region.
      void setRandomTerrainEnabledState(bool);     // Enable or disable random terrain wrapping.
      int chunksAwaitingUpdate();                  // The number of stale chunks.
   private:
      vector3i voxelCoordToVoxelIndex(vector3i); // Translate world voxel coords to voxel indices (chunk space).
      vector3i voxelCoordToChunkIndex(vector3i); // Translate world voxel coords to chunk indices (world 'renderable zone' space).
      glm::vec3 voxelCoordToChunkIndex(glm::vec3); // Translate world voxel coords to chunk indices (world 'renderable zone' space).
      glm::vec3 voxelCoordToChunkCoord(glm::vec3 coord); // Translate voxel world coords to chunk coords.
      vector3i chunkCoordToChunkIndex(vector3i);   // Translate chunk world coords to chunk render zone indices.
      bool useFastMeshBuilder;                     // Use fast mesh generation.
      void chunkUpdateQuery();                     // Check all chunks to see which ones need an update.
      int worldViewDistance;                       // How far you can see measured in voxels.
      glm::vec3 lastCamPosition;                   // The last known position of the camera.
      void camPositionCheck();                     // If the camera has moved into a new chunk we may need to draw a new part of the world.
      glm::vec3 worldCentre;                       // The chunk that is central in the world.
      bool xWrappingEnabled;                       // The world is infinite in the x-axis.
      bool yWrappingEnabled;                       // The world is infinite in the y-axis.
      bool zWrappingEnabled;                       // The world is infinite in the z-axis.
      PerlinNoise* terrainGen;
      bool randomTerrainEnabled;                   // Flag to indicate whether or not random terrain should be loaded into new regions.
};