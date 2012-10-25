// OpenGL Mathematics Library
#include <glm/glm.hpp>
#include <vector>
#include "chunk.cpp"    // Defines a chunk class which acts as a voxel container
                        // and provides utility functions for accessing or 
                        // modifying the data as well as rendering the data. 
#include "perlin.cpp"
                                   
class World
{
   public:
      World(int, int, int, int, bool, bool, bool, GLuint); 
      ~World();                                    // Destruct
      void draw(glm::vec3, glm::mat4);             // Draw the world.
      void fillSpheres();                          // Fill world with spheres.
      void sphere();                               // Fill with world with a sphere.
      void fillPyramids();                         // Fill world with pyramids.
      void fill();                                 // Fill world.
      void random();                               // Random world.
      bool exists(int, int, int);                  // Chunk x,y,z exists.
      bool exists(vector3i);                       // Chunk x,y,z exists.
      void deleteBlockAt(int, int, int);           // Delete voxel x,y,z.                          
      void load(byte*, int, int, int, int);        // Load world with 3D array.
      void loadHeightmap(byte*, int);              // Load world with heightmap.
      void setViewDistance(int);                   // Set the viewing distance.
      void loadRegion(int, int);                   // Load given x,z region.
      void setRandomTerrainEnabledState(bool);     // Enable random terrain gen.
      int chunksAwaitingUpdate();                  // Number of stale chunks.
      int getNumVertices();                        // Number of vertices.
      // Delete a spherical region of voxels at the
      // specified world coord with the specified radius.
      void modifyRegionAt(int, int, int, byte, int);  
      vector3i worldDimensions();                  // Get the world dimensions.
      int worldChunkSize();                        // Get the world chunk size.
      bool is_solid(int,int,int);                  // Voxel at x,y,z is solid.
   private:
      /* Variables */
      
      // Chunk Storage
      std::map<vector3i,Chunk*> chunks;
      // Processing Queues
      std::vector<Chunk*> chunkUpdateQueue;        // Mesh updates.
      //std::vector<Chunk*> chunkOptimiseQueue;    // Mesh Optimisation. (Todo).
      std::vector<Chunk*> region2DLoaderQueue;     // Data load.
      // Properties
      vector3i dim;                                // World dimensions (chunks).
      int chunk_size;                              // Chunk dimensions (voxels).
      int worldHeight;                             // World height in voxels.
      int vertices;                                // World vertices.
      glm::vec3 lastCamPosition;                   // Last camera position.
      bool useFastMeshBuilder;                     // Use fast mesh generation.
      int worldViewDistance;                       // View distance (voxels).
      glm::vec3 worldCentre;                       // Central chunk. 
      bool xWrappingEnabled;                       // Infinite x-axis.
      bool yWrappingEnabled;                       // Infinite y-axis.
      bool zWrappingEnabled;                       // Infinite z-axis.
      PerlinNoise* terrainGen;                     // Random terrain generator.
      bool randomTerrainEnabled;                   // Enable random terrain.
      GLuint renderProgram;                        // OpenGL shader program id.
      GLuint worldHeightAttrib;
      
      /* Functions */
      
      // Translate world voxel coords to voxel indices (chunk space).
      vector3i voxelCoordToVoxelIndex(vector3i); 
      // Translate world voxel coords to chunk indices.
      vector3i voxelCoordToChunkIndex(vector3i); 
      // Translate world voxel coords to chunk indices.
      glm::vec3 voxelCoordToChunkIndex(glm::vec3); 
      // Translate voxel world coords to chunk coords.
      glm::vec3 voxelCoordToChunkCoord(glm::vec3 coord); 
      // Translate chunk world coords to chunk render zone indices.
      vector3i chunkCoordToChunkIndex(vector3i);      
      // Check all chunks to see which ones need an update.
      void chunkUpdateQuery();          
      // Check to see if any chunks need wrapping or loading.
      void camPositionCheck(); 
};