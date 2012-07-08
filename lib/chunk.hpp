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
   smallVolume chunkData;                    // Map of bytes representing voxel volume.
         
   faceGroup visibleFaceGroups;              // Which face groups are visible.
   
   vector3i dim;                             // The dimensions of the chunk
   glm::vec3 pos;                            // The position of the chunk.
   glm::vec3 centre;                         // The position of the chunk centre.
   int size;                                 // The size of the volume.
   int worldHeight;                          // The height of the world in voxels. 
   
   GLuint verticesFrontVBO;                  // Vertex Buffer Object for chunk faces.
   GLuint verticesBackVBO;                   // Vertex Buffer Object for chunk faces.
   GLuint verticesLeftVBO;                   // Vertex Buffer Object for chunk faces.
   GLuint verticesRightVBO;                  // Vertex Buffer Object for chunk faces.
   GLuint verticesAboveVBO;                  // Vertex Buffer Object for chunk faces.
   GLuint verticesBelowVBO;                  // Vertex Buffer Object for chunk faces.
   
   GLuint normVBO;                           // Normals VBO for the chunk.
   GLint posAttrib; 
   GLuint normAttrib;
   GLuint worldPosAttrib;
   GLuint worldHeightAttrib;
   unsigned int voxels;
   
   bool firstDrawCall;                       // Flag to enable one-time draw operations.
      
   public:
      std::vector<vertex> verticesFront;     // Array of vertices: front faces.
      std::vector<vertex> verticesBack;      // Array of vertices: back faces.
      std::vector<vertex> verticesLeft;      // Array of vertices: left faces.
      std::vector<vertex> verticesRight;     // Array of vertices: right faces.
      std::vector<vertex> verticesAbove;     // Array of vertices: above faces.
      std::vector<vertex> verticesBelow;     // Array of vertices: below faces.
      std::vector<vertex> verticesFrontBuf;  // Temp holding buffer.
      std::vector<vertex> verticesBackBuf;   // Temp holding buffer.
      std::vector<vertex> verticesLeftBuf;   // Temp holding buffer.
      std::vector<vertex> verticesRightBuf;  // Temp holding buffer.
      std::vector<vertex> verticesAboveBuf;  // Temp holding buffer.
      std::vector<vertex> verticesBelowBuf;  // Temp holding buffer.
      
      int verticesRenderedCount;             // The number of vertices rendered in this volume.
   
   
      Chunk(int, int, int, int, int);        // Construct
      ~Chunk();                              // Destruct
      void load(byte*, int);                 // Load external data into the chunk.
      byte get(int, int, int);               // Get the value of a specific voxel.
      void set(int, int, int, byte);         // Set the value of a specific voxel.
      void setHeight(int, int, int);
      void fill();                           // Completely fill the chunk with solid voxels.
      void empty();                          // Empty the chunk.   
      void meshBuilderSlow();                // Build an optimised mesh.
      void meshBuilderFast();                // Build an unoptimised mesh.
      void initialiseMeshBuilder();          // Initialise the mesh builder.
      glm::vec3 position();                  // Get the chunk position.
      bool requireMeshUpdate();              // Test if the chunk mesh is out of date.
      bool meshBuildRunning();               // Test if the mesh builder is running.
      // Render the chunk.
      void draw(GLuint, glm::vec3, glm::mat4, int);  
      void update(bool);                     // Update display buffers for the chunk.
      void setVisibleFaceGroup(glm::vec3);   // Move to world class when its created.
      void buildDisplayList();               // Build the display list vector.
      unsigned int voxelcount();             // Return the number of voxels contained in the chunk.
      void initDraw(GLuint);
      void setChunkPosition(int, int, int);  // Update the chunk world coords.
   private:
      bool modified;                         // Flag to indicate the chunk data has been modified. 
      bool visible;                          // Flag to indicate if the chunk is visible.
      bool meshBuildInProgress;              // Flag to indicate if the mesh is out of date.

      void addFace(int, int, int, facePos, int); // Specific to the positions array.
      void addFace2(int, int, int, int, int, int,
                   facePos, byte);              // Add a face to the vertex list.
      // Iterator used by the mesh builder.
      boost::unordered_map<Position, block>::iterator ii;
};
