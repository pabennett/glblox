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
   public:
      int vertices;                                // The number of vertices required to draw the world.
      World(int, int, int, int, bool);             // Construct
      ~World();                                    // Destruct
      void draw(GLuint, glm::vec3, glm::mat4);     // Draw the world.
      void fillSpheres();                          // Fill the world with spheres.
      void fill();                                 // Fill the world.
      void random();                               // Randomly generated world.
      void load(byte*, int, int, int, int);
   private:
      bool useFastMeshBuilder;                     // Use fast mesh generation.
};