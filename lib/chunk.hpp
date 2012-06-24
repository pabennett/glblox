#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <GL/wglew.h>

#include <iostream>
#include <cstring>
#include <new>
#include <vector>
#include <boost/unordered_map.hpp>
// OpenGL Mathematics Library
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp> 
#include "timeit.cpp"
#include "simplex.c"
#include "smallVolume.cpp"

enum facePos { ABOVE, BELOW, LEFT, RIGHT, FRONT, BACK };
enum xfacePos {DRAW_LEFT, DRAW_RIGHT, DRAW_BOTH_X};
enum yfacePos {DRAW_ABOVE, DRAW_BELOW, DRAW_BOTH_Y};
enum zfacePos {DRAW_FRONT, DRAW_BACK, DRAW_BOTH_Z};

typedef struct{
   xfacePos x; yfacePos y; zfacePos z;
} faceGroup;

// Vertices are stored as 8bit unsigned.
// They are modified in the vertex shader to apply the chunk world offset.
typedef struct{
   byte x, y, z, w;
} vertex;

class Chunk
{
   smallVolume chunkData;                       // Map of bytes representing voxel volume.
         
   faceGroup visibleFaceGroups;                 // Which face groups are visible.
   
   vector3i dim;                                // The dimensions of the chunk
   glm::vec3 pos;                               // The position of the chunk.
   glm::vec3 centre;                            // The position of the chunk centre.
   int size;                                    // The size of the volume.
   int worldHeight;                             // The height of the world in voxels. 
   
   GLuint verticesFrontVBO;                     // Vertex Buffer Object for chunk faces.
   GLuint verticesBackVBO;                      // Vertex Buffer Object for chunk faces.
   GLuint verticesLeftVBO;                      // Vertex Buffer Object for chunk faces.
   GLuint verticesRightVBO;                     // Vertex Buffer Object for chunk faces.
   GLuint verticesAboveVBO;                     // Vertex Buffer Object for chunk faces.
   GLuint verticesBelowVBO;                     // Vertex Buffer Object for chunk faces.
   
   GLuint normVBO;                              // Normals VBO for the chunk.
   GLint posAttrib; 
   GLuint normAttrib;
   GLuint worldPosAttrib;
   GLuint worldHeightAttrib;
   unsigned int voxels;
   
   bool firstDrawCall;                          // Flag to enable one-time draw operations.
      
   public:
      std::vector<vertex> verticesFront;        // Array holding all triangles on front faces.
      std::vector<vertex> verticesBack;         // Array holding all triangles on back faces.
      std::vector<vertex> verticesLeft;         // Array holding all triangles on left faces.
      std::vector<vertex> verticesRight;        // Array holding all triangles on right faces.
      std::vector<vertex> verticesAbove;        // Array holding all triangles on above faces.
      std::vector<vertex> verticesBelow;        // Array holding all triangles on below faces.
      int verticesRenderedCount;                // The number of vertices rendered in this volume.
   
   
      Chunk(int, int, int, int, int);           // Construct
      ~Chunk();                                 // Destruct
      void load(byte*, int);                    // Load external data into the chunk.
      byte get(int, int, int);                  // Get the value of a specific voxel.
      void set(int, int, int, byte);            // Set the value of a specific voxel.
      void fill();                              // Completely fill the chunk with solid voxels.
      void empty();                             // Empty the chunk.   
      void meshBuilderSlow();                   // Build an optimised mesh.
      void meshBuilderFast();                   // Build an unoptimised mesh.
      void clearModifiedState();                // Clear the modified flag.
      bool is_modified();                       // Test if the chunk has been modified.
      void draw(GLuint, glm::vec3, glm::mat4, bool);  // Render the chunk.
      void update(bool);                            // Update display buffers for the chunk.
      void setVisibleFaceGroup(glm::vec3);      // Move to world class when its created.
      void buildDisplayList();                  // Build the display list vector.
      unsigned int voxelcount();                // Return the number of voxels contained in the chunk.
      void initDraw(GLuint);
   private:
      bool modified;                               // Flag to indicate the chunk data has been modified. 

      void setPos(int, int, int, facePos); // Specific to the positions array.
      void addFace(int, int, int, int, int, int,
                   facePos, byte);              // Add a face to the vertex list.
};
