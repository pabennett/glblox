#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>

#include <iostream>
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

class Chunk
{                      
   public:      
      // Construct
      Chunk(int, int, int, int, int, GLuint);        
      // Destruct
      ~Chunk();                              
      // Load external data into the chunk.
      void load(byte*, int);                 
      // Get the value of a specific voxel.
      byte get(int, int, int);  
      // Return true if the given voxel is solid.
      bool is_solid(int, int, int);
      bool is_solid(vector3i);
      // Set the value of a specific voxel.
      void set(int, int, int, byte);         
      void setHeight(int, int, int);
      // Completely fill the chunk with solid voxels.
      void fill();    
      // Empty the chunk. 
      void empty();   
      // Initialise the mesh builder.
      void initialiseMeshBuilder(); 
      // Get the chunk position.      
      glm::vec3 position();
      // Get the chunk centre.
      glm::vec3 getCentre();
      // Test if the chunk mesh is out of date.      
      bool requireMeshUpdate();    
      // Test if the mesh builder is running.
      bool meshBuildRunning();               
      // Render the chunk.
      void draw(GLuint);                        
      // Update display buffers for the chunk - calls mesh gen.   
      int update(bool,int);      
      // TODO: Implement this in the world class...
      void setVisibleFaceGroup(faceGroup);   
      // Build the display list vector.
      void buildDisplayList();
      // Update the chunk world coords.
      void setChunkPosition(int, int, int);  
      // Return true if the chunk is compressed.
      bool is_compressed();     
      // Uncompress the chunk data if it is compressed.
      void uncompress();   
      // Return the vertex count.
      int getVertexCount();   
      // Mark as stale (requires mesh rebuild)
      void markStale();
      void markNeighbourStale(int, int, int);
      // Set pointer to neighbour chunk.
      void setLeftNeighbour(Chunk*);                  
      void setRightNeighbour(Chunk*);                 
      void setTopNeighbour(Chunk*);                   
      void setBottomNeighbour(Chunk*);                
      void setFrontNeighbour(Chunk*);                 
      void setBackNeighbour(Chunk*);  
      bool rightBorderFull();
      bool leftBorderFull();
      bool topBorderFull();
      bool bottomBorderFull();
      bool frontBorderFull();
      bool backBorderFull();
      bool leftNeighbourIsSolid(int, int, int);
      bool rightNeighbourIsSolid(int, int, int);
      bool topNeighbourIsSolid(int, int, int);
      bool bottomNeighbourIsSolid(int, int, int);
      bool frontNeighbourIsSolid(int, int, int);
      bool backNeighbourIsSolid(int, int, int);
   private:
      /* Variables */
      
      // Voxel Storage
      smallVolume chunkData;                 // (boost::unordered_map)
      // Vertex storage
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
      // OpenGL VBOS
      GLuint verticesFrontVBO;               // VBO for chunk faces.
      GLuint verticesBackVBO;                // VBO for chunk faces.
      GLuint verticesLeftVBO;                // VBO for chunk faces.
      GLuint verticesRightVBO;               // VBO for chunk faces.
      GLuint verticesAboveVBO;               // VBO for chunk faces.
      GLuint verticesBelowVBO;               // VBO for chunk faces.
      // OpenGL Attributes
      GLint posAttrib;
      GLuint normAttrib;
      GLuint worldPosAttrib;
      // Neighbour chunk references
      Chunk* leftNeighbour;                  // Pointer to neighbour chunk.
      Chunk* rightNeighbour;                 // Pointer to neighbour chunk.
      Chunk* topNeighbour;                   // Pointer to neighbour chunk.
      Chunk* bottomNeighbour;                // Pointer to neighbour chunk.
      Chunk* frontNeighbour;                 // Pointer to neighbour chunk.
      Chunk* backNeighbour;                  // Pointer to neighbour chunk.
      // Flags
      bool modified;                         // Chunk data has been modified. 
      bool visible;                          // Chunk is visible.
      bool meshBuildInProgress;              // Mesh builder incomplete.
      faceGroup visibleFaceGroups;           // Which face groups are visible.
      // Properties
      int verticesRenderedCount;             // Number of vertices in chunk.
      int chunk_size;                        // The volume dimensions (square).
      int worldHeight;                       // World height (in voxels). 
      glm::vec3 pos;                         // Position of the chunk.
      glm::vec3 centre;                      // Position of the chunk centre.
      int meshGenWorkAllowance;              // Iterations/frame for mesh gen.
      // Iterators
      boost::unordered_map<Position, block>::iterator ii;
      
      /* Functions */
      
      // Add a voxel face to the temporary buffers (used by mesh builder).
      void addFace(int, int, int, facePos, int); 
      // Build an unoptimised mesh.
      int meshBuilderFast(); 
      bool obscuredByChunkNeighbours();
};
