////////////////////////////////////////////////////////////////////////////////
// Chunk          Provides storage for voxel data and functions to modify or
//                read the voxel data. Functions to render the volume data
//                as a triangular mesh are also provided.
// 
// @author         Peter A Bennett
// @copyright      (c) 2012 Peter A Bennett
// @license        LGPL      
// @email          pab850@googlemail.com
// @contact        www.bytebash.com
//
////////////////////////////////////////////////////////////////////////////////

#include "chunk.hpp"

Chunk::Chunk(int chunk_x, int chunk_y, int chunk_z,
             int chunk_size, 
             int wh,
             GLuint program) : chunkData(chunk_size)
{
   worldHeight = wh;
   
   Chunk::chunk_size = chunk_size;
   pos.x = chunk_x;
   pos.y = chunk_y;
   pos.z = chunk_z;
   centre.x = pos.x + 0.5 * chunk_size;
   centre.y = pos.y + 0.5 * chunk_size;
   centre.z = pos.z + 0.5 * chunk_size;
   
   visibleFaceGroups.x = DRAW_BOTH_X;
   visibleFaceGroups.y = DRAW_BOTH_Y;
   visibleFaceGroups.z = DRAW_BOTH_Z;
   
   std::cout << "CPP: Generating chunk with position " << pos.x << "," 
                                                       << pos.y << "," 
                                                       << pos.z << std::endl;
   modified = false;
   visible = false;
   meshBuildInProgress = false;
   // Init VBO
   glGenBuffers(1, &verticesFrontVBO);
   glGenBuffers(1, &verticesBackVBO);
   glGenBuffers(1, &verticesLeftVBO);
   glGenBuffers(1, &verticesRightVBO);
   glGenBuffers(1, &verticesAboveVBO);
   glGenBuffers(1, &verticesBelowVBO);
   // Init attributes
	glUseProgram(program);
   posAttrib = glGetAttribLocation(program, "position");
   normAttrib = glGetUniformLocation(program, "normal");
   worldPosAttrib = glGetUniformLocation(program, "worldPosition");
	glUseProgram(0);
}

Chunk::~Chunk()
{
   std::cout << "CPP: Chunk object destroyed." << std::endl;
}

// Update the chunk position in the world (voxel coords).
void Chunk::setChunkPosition(int x, int y, int z)
{
   pos.x = x;
   pos.y = y;
   pos.z = z;
   centre.x = pos.x + 0.5 * chunk_size;
   centre.y = pos.y + 0.5 * chunk_size;
   centre.z = pos.z + 0.5 * chunk_size;
}

// Get the chunk position.
glm::vec3 Chunk::position()
{
   return pos;
}

// Get the chunk centre.
glm::vec3 Chunk::getCentre()
{
   return centre;
}

// Set which faces are visible (TODO: Move to World class)
void Chunk::setVisibleFaceGroup(glm::vec3 camPosition)
{  

   vector3i cam;
   cam.x = (int)camPosition.x;
   cam.y = (int)camPosition.y;
   cam.z = (int)camPosition.z;
   bool rebuildDisplayList = false;
   verticesRenderedCount = 0;
   if(cam.x < pos.x)
   {
      verticesRenderedCount += verticesLeft.size();
      if(visibleFaceGroups.x != DRAW_LEFT)
      {
         visibleFaceGroups.x = DRAW_LEFT;
         rebuildDisplayList = true;
      }
   }
   else if(cam.x > (pos.x + chunk_size))
   {
      verticesRenderedCount += verticesRight.size();
      if(visibleFaceGroups.x != DRAW_RIGHT)
      {
         visibleFaceGroups.x = DRAW_RIGHT;
         rebuildDisplayList = true;
      }
   }
   else 
   {
      verticesRenderedCount += verticesLeft.size();
      verticesRenderedCount += verticesRight.size();
      if(visibleFaceGroups.x != DRAW_BOTH_X)
      {
         visibleFaceGroups.x = DRAW_BOTH_X;
         rebuildDisplayList = true;
      }
   }
   if(cam.y < pos.y)
   {
      verticesRenderedCount += verticesBelow.size();
      if(visibleFaceGroups.y != DRAW_BELOW)
      {
         visibleFaceGroups.y = DRAW_BELOW;
         rebuildDisplayList = true;
      }
   }
   else if(cam.y > (pos.y + chunk_size))
   {
      verticesRenderedCount += verticesAbove.size();
      if(visibleFaceGroups.y != DRAW_ABOVE)
      {
         visibleFaceGroups.y = DRAW_ABOVE;
         rebuildDisplayList = true;
      }
   }
   else 
   {
      verticesRenderedCount += verticesAbove.size();
      verticesRenderedCount += verticesBelow.size();
      if(visibleFaceGroups.y != DRAW_BOTH_Y)
      {
         visibleFaceGroups.y = DRAW_BOTH_Y;
         rebuildDisplayList = true;
      }
   }
   if(cam.z < pos.z)
   {
      verticesRenderedCount += verticesBack.size();
      if(visibleFaceGroups.z != DRAW_BACK)
      {
         visibleFaceGroups.z = DRAW_BACK;
         rebuildDisplayList = true;
      }
   }
   else if(cam.z > (pos.z + chunk_size))
   {
      verticesRenderedCount += verticesFront.size();
      if(visibleFaceGroups.z != DRAW_FRONT)
      {
         visibleFaceGroups.z = DRAW_FRONT;
         rebuildDisplayList = true;
      }
   }
   else 
   {
      verticesRenderedCount += verticesFront.size();
      verticesRenderedCount += verticesBack.size();
      if(visibleFaceGroups.z != DRAW_BOTH_Z)
      {
         visibleFaceGroups.z = DRAW_BOTH_Z;
         rebuildDisplayList = true;
      }
   }
   if(rebuildDisplayList)
   {
      rebuildDisplayList = false;
      buildDisplayList();
   }
}

// Bind the appropriate vertex arrays if they are visible and contain data.
void Chunk::buildDisplayList()
{
   if(!verticesFront.empty() and (visibleFaceGroups.z == DRAW_FRONT or visibleFaceGroups.z == DRAW_BOTH_Z))
   {
      glBindBuffer(GL_ARRAY_BUFFER, verticesFrontVBO);
      glBufferData(GL_ARRAY_BUFFER, verticesFront.size() * sizeof(vertex), &verticesFront[0], GL_STATIC_DRAW);
   }
   if(!verticesBack.empty() and (visibleFaceGroups.z == DRAW_BACK or visibleFaceGroups.z == DRAW_BOTH_Z))
   {
      glBindBuffer(GL_ARRAY_BUFFER, verticesBackVBO);
      glBufferData(GL_ARRAY_BUFFER, verticesBack.size() * sizeof(vertex), &verticesBack[0], GL_STATIC_DRAW);
   }
   if(!verticesLeft.empty() and (visibleFaceGroups.x == DRAW_LEFT or visibleFaceGroups.x == DRAW_BOTH_X))
   {
      glBindBuffer(GL_ARRAY_BUFFER, verticesLeftVBO);
      glBufferData(GL_ARRAY_BUFFER, verticesLeft.size() * sizeof(vertex), &verticesLeft[0], GL_STATIC_DRAW);
   }
   if(!verticesRight.empty() and (visibleFaceGroups.x == DRAW_RIGHT or visibleFaceGroups.x == DRAW_BOTH_X))
   {
      glBindBuffer(GL_ARRAY_BUFFER, verticesRightVBO);
      glBufferData(GL_ARRAY_BUFFER, verticesRight.size() * sizeof(vertex), &verticesRight[0], GL_STATIC_DRAW);
   }
   if(!verticesAbove.empty() and (visibleFaceGroups.y == DRAW_ABOVE or visibleFaceGroups.y == DRAW_BOTH_Y))
   {
      glBindBuffer(GL_ARRAY_BUFFER, verticesAboveVBO);
      glBufferData(GL_ARRAY_BUFFER, verticesAbove.size() * sizeof(vertex), &verticesAbove[0], GL_STATIC_DRAW);
   }
   if(!verticesBelow.empty() and (visibleFaceGroups.y == DRAW_BELOW or visibleFaceGroups.y == DRAW_BOTH_Y))
   {
      glBindBuffer(GL_ARRAY_BUFFER, verticesBelowVBO);
      glBufferData(GL_ARRAY_BUFFER, verticesBelow.size() * sizeof(vertex), &verticesBelow[0], GL_STATIC_DRAW);
   }
   glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Update the mesh to reflect volume changes.
int Chunk::update(bool useFastMeshBuilder, int buildCycles)
{
   // Iterate through the volume and generate a mesh.
   int64 time = GetTimeMs64();
   meshGenWorkAllowance = buildCycles;
   int workDone = 1;
   if(!visible)
   {
      verticesLeft.clear();   
      verticesRight.clear();
      verticesAbove.clear();
      verticesBelow.clear();
      verticesFront.clear();
      verticesBack.clear();
      meshBuildInProgress = false;
   }
   else
   {

      workDone = meshBuilderFast();

      // If the update completed this cycle rebuild the display list.
      if(!meshBuildInProgress)
      {
         buildDisplayList();
      }
   }
   return workDone;
}

// Return the number of vertices in this chunk.
int Chunk::getVertexCount()
{
   return verticesRenderedCount;
}

// Draw the chunk mesh.
void Chunk::draw(GLuint program, 
                 glm::vec3 camPosition)
{
   // If there is nothing to draw then abort early.
   if(!visible)
   {
      return;
   }
         
	glUseProgram(program);
   setVisibleFaceGroup(camPosition);
         
   // Tell GLSL the world coords of this chunk. 
   glUniform3fv(worldPosAttrib, 1, glm::value_ptr(pos));

   glEnableVertexAttribArray(posAttrib);   
   if(visibleFaceGroups.z == DRAW_FRONT or visibleFaceGroups.z == DRAW_BOTH_Z)
   {
      glUniform3fv(normAttrib, 1, glm::value_ptr(glm::vec3(0, 0, -1)));
      glBindBuffer(GL_ARRAY_BUFFER, verticesFrontVBO);
      glVertexAttribPointer(posAttrib, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
      glDrawArrays(GL_TRIANGLES, 0, verticesFront.size());
   }
   if(visibleFaceGroups.z == DRAW_BACK or visibleFaceGroups.z == DRAW_BOTH_Z)
   {
      glUniform3fv(normAttrib, 1, glm::value_ptr(glm::vec3(0, 0, 1)));
      glBindBuffer(GL_ARRAY_BUFFER, verticesBackVBO);
      glVertexAttribPointer(posAttrib, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
      glDrawArrays(GL_TRIANGLES, 0, verticesBack.size());
   }
   if(visibleFaceGroups.x == DRAW_LEFT or visibleFaceGroups.x == DRAW_BOTH_X)
   {
      glUniform3fv(normAttrib, 1, glm::value_ptr(glm::vec3(1, 0, 0)));
      glBindBuffer(GL_ARRAY_BUFFER, verticesLeftVBO);
      glVertexAttribPointer(posAttrib, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
      glDrawArrays(GL_TRIANGLES, 0, verticesLeft.size());
   }
   if(visibleFaceGroups.x == DRAW_RIGHT or visibleFaceGroups.x == DRAW_BOTH_X)
   {
      glUniform3fv(normAttrib, 1, glm::value_ptr(glm::vec3(-1, 0, 0)));
      glBindBuffer(GL_ARRAY_BUFFER, verticesRightVBO);
      glVertexAttribPointer(posAttrib, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
      glDrawArrays(GL_TRIANGLES, 0, verticesRight.size());
   }
   if(visibleFaceGroups.y == DRAW_ABOVE or visibleFaceGroups.y == DRAW_BOTH_Y)
   {
      glUniform3fv(normAttrib, 1, glm::value_ptr(glm::vec3(0, -1, 0)));
      glBindBuffer(GL_ARRAY_BUFFER, verticesAboveVBO);
      glVertexAttribPointer(posAttrib, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
      glDrawArrays(GL_TRIANGLES, 0, verticesAbove.size());
   }
   if(visibleFaceGroups.y == DRAW_BELOW or visibleFaceGroups.y == DRAW_BOTH_Y)
   {
      glUniform3fv(normAttrib, 1, glm::value_ptr(glm::vec3(0, 1, 0)));
      glBindBuffer(GL_ARRAY_BUFFER, verticesBelowVBO);
      glVertexAttribPointer(posAttrib, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
      glDrawArrays(GL_TRIANGLES, 0, verticesBelow.size());
   }
   glDisableVertexAttribArray(posAttrib);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}
 
// Fill the chunk.
void Chunk::fill()
{
   chunkData.fill();
   modified = true;
   visible = true;
}

// Empty the chunk
void Chunk::empty()
{
   chunkData.empty();
   modified = true;
   visible = false;
}

// Find out if the chunk is compressed.
bool Chunk::is_compressed()
{
   return chunkData.is_compressed();
}

// Uncompress the chunk if it is compressed.
void Chunk::uncompress()
{
   if(is_compressed())
   {
      chunkData.uncompress();
   }
}

// Load a 3D array of bytes into the chunk.
void Chunk::load(byte* initialiser, int chunk_size)
{
   int x,y,z;
   if (chunk_size != Chunk::chunk_size)
   {
      std::cout << "The data array does not match the chunk size! Aborting load." << std::endl;
   }
   else
   {
      chunkData.empty();
      for (x = 0; x < chunk_size; x++)
      {
         for (y = 0; y < chunk_size; y++)
         {
            for (z = 0; z < chunk_size; z++)
            {
               chunkData.set(x,y,z,initialiser[x + (y * chunk_size) + 
                                              (z * chunk_size * chunk_size)]);
            }
         }
      }
   }
   modified = chunkData.is_modified();
   visible = !chunkData.is_empty();
   chunkData.clearModifiedState();
}

// Set an arbitrary voxel in the chunk.
void Chunk::set(int x, int y, int z, byte initialiser)
{
   chunkData.set(x,y,z,initialiser);
   modified = chunkData.is_modified() or modified;
   if(chunkData.is_modified())
   {
      markNeighbourStale(x,y,z);
   }
   visible = !chunkData.is_empty();
   chunkData.clearModifiedState();
}

void Chunk::markNeighbourStale(int x, int y, int z)
{
   // If a border voxel is set mark the neighbour chunk as stale.
   if(x == 0 and leftNeighbour != 0)
   {
      leftNeighbour->markStale();
   }
   else if(x == chunk_size - 1 and rightNeighbour != 0)
   {
      rightNeighbour->markStale();
   }
   
   if(z == 0 and backNeighbour != 0)
   {
      backNeighbour->markStale();
   }
   else if(z == chunk_size - 1 and frontNeighbour != 0)
   {
      frontNeighbour->markStale();
   }
   
   if(y == 0 and bottomNeighbour != 0)
   {
      bottomNeighbour->markStale();
   }
   else if(y == chunk_size - 1 and topNeighbour != 0)
   {
     topNeighbour->markStale();
   }
}

void Chunk::markStale()
{
   modified = true;
}

// Get the value of an arbitrary voxel in the chunk.
byte Chunk::get(int x, int y, int z)
{
   return chunkData.get(x,y,z);
}

// Create a pillar of voxels at x,z with height h.
void Chunk::setHeight(int x, int z, int h)
{
   int y;
   // Clamp height values.
   h = h >= chunk_size ? chunk_size - 1 : h;
   if(h < 0)
   {
      return;
   }
   chunkData.yRangeSet(x,0,h,z,1);
   modified = chunkData.is_modified() or modified;
   visible = !chunkData.is_empty();
   chunkData.clearModifiedState();
}

// Called by the higher level to indicate that the modified state of this chunk
// has been acknowledged. Mark the mesh used by this chunk as stale so that the
// mesh builder can reconstruct it over time.
void Chunk::initialiseMeshBuilder()
{
   modified = false;
   ii=chunkData.begin();
   meshBuildInProgress = true;
}

// Return true if the chunk is stale and requires a mesh rebuild.
bool Chunk::requireMeshUpdate()
{
   return not meshBuildInProgress and modified and visible;
}

// Return true if the mesh builder is running.
bool Chunk::meshBuildRunning()
{
   return meshBuildInProgress;
}

// Generate a triangle mesh which can be used to represent
// the voxel volume as a regular grid of cubes.
// Returns the number of iterations that were performed.
int Chunk::meshBuilderFast()
{
   int x,y,z;              // Chunk x,y,z
   // Have the volume contents changed? Restart the mesh builder.
   if(modified)
   {
      modified = false;
      ii=chunkData.begin();
   }
   // Has the mesh builder just started?
   if(ii==chunkData.begin())
   {
      // Mesh builder has just been started, clear the vertex buffers.
      verticesLeftBuf.clear();   
      verticesRightBuf.clear();
      verticesAboveBuf.clear();
      verticesBelowBuf.clear();
      verticesFrontBuf.clear();
      verticesBackBuf.clear();
      // First check to see if this chunk is completely surrounded.
      if(obscuredByChunkNeighbours())
      {
         // This chunk is obscured, dont draw it.
         verticesLeft.clear();   
         verticesRight.clear();
         verticesAbove.clear();
         verticesBelow.clear();
         verticesFront.clear();
         verticesBack.clear();
         meshBuildInProgress = false;
         return 1;
      }
      // Check to see if the chunk is completely full.
      if(chunkData.is_full())
      {
         // The chunk is full, create a primitive mesh and terminate mesh build.
         addFace(0, 0, 0, LEFT, chunk_size);
         addFace(0, 0, 0, RIGHT, chunk_size);
         addFace(0, 0, 0, ABOVE, chunk_size);
         addFace(0, 0, 0, BELOW, chunk_size);
         addFace(0, 0, 0, BACK, chunk_size);
         addFace(0, 0, 0, FRONT, chunk_size);
         verticesLeft.swap(verticesLeftBuf);  
         verticesRight.swap(verticesRightBuf);
         verticesAbove.swap(verticesAboveBuf);
         verticesBelow.swap(verticesBelowBuf);
         verticesFront.swap(verticesFrontBuf);
         verticesBack.swap(verticesBackBuf);
         meshBuildInProgress = false;
         return 1;
      }
   }

   // The chunk is not full, build the mesh normally.  
   // Loop through every voxel in the volume and create the vertices required
   // to render exposed faces. Faces hidden by surrounding cubes are not drawn.
   
   // The mesh builder is only allowed to perform a certain amount of work
   // before it terminates and must be called again. This helps keep the frame
   // rate fluid but it does add latency to mesh updates.
   for(int i = 0; i < meshGenWorkAllowance; i++)
   {
      if(ii==chunkData.end())
      {
         // Mesh generation complete.
         verticesLeft.swap(verticesLeftBuf);  
         verticesRight.swap(verticesRightBuf);
         verticesAbove.swap(verticesAboveBuf);
         verticesBelow.swap(verticesBelowBuf);
         verticesFront.swap(verticesFrontBuf);
         verticesBack.swap(verticesBackBuf);
         meshBuildInProgress = false;
         ii=chunkData.begin();
         return i;
      }
      // Get the voxel chunk coords.
      x = (*ii).first.tuple.get<0>();
      y = (*ii).first.tuple.get<1>();
      z = (*ii).first.tuple.get<2>();
      // Check to see if the neighbours are empty and draw faces.
      if(x > 0)
      {
         if(chunkData.blockLeftVisible(x,y,z))
         {
            addFace(x, y, z, LEFT, 1);
         }
      }
      else if(not leftNeighbourIsSolid(chunk_size-1,y,z))
      {
         addFace(x, y, z, LEFT, 1);
      }
      
      if(x < chunk_size - 1)
      {
         if(chunkData.blockRightVisible(x,y,z))
         {
            addFace(x, y, z, RIGHT, 1);
         }
      }
      else if(not rightNeighbourIsSolid(0,y,z))
      {
         addFace(x, y, z, RIGHT, 1);
      }
      
      if(y > 0)
      {
         if(chunkData.blockBelowVisible(x,y,z))
         {
            addFace(x, y, z, BELOW, 1);
         }
      }
      else if(not bottomNeighbourIsSolid(x,chunk_size-1,z))
      {
         addFace(x, y, z, BELOW, 1);
      }
        
      if(y < chunk_size - 1)
      {
         if(chunkData.blockAboveVisible(x,y,z))
         {
            addFace(x, y, z, ABOVE, 1);
         }
      }
      else if(not topNeighbourIsSolid(x,0,z))
      {
         addFace(x, y, z, ABOVE, 1);
      }
      
      if(z > 0)
      {
         if(chunkData.blockBackVisible(x,y,z))
         {
            addFace(x, y, z, BACK, 1);
         }
      }
      else if(not backNeighbourIsSolid(x,y,chunk_size-1))
      {
         addFace(x, y, z, BACK, 1);
      }
      
      if(z < chunk_size - 1)
      {
         if(chunkData.blockFrontVisible(x,y,z))
         {
            addFace(x, y, z, FRONT, 1);
         }
      }
      else if(not frontNeighbourIsSolid(x,y,0))
      {
         addFace(x, y, z, FRONT, 1);
      }
      ++ii;
   }
   return meshGenWorkAllowance;
}

// Create a cube face for the given voxel.

/*  
              Above      Front
                  |     / 
              +-------+
             /|      /|
            / |     / |
           /  |    /  |
  Left--  +---|---+   |  --Right
          |   +---|---+
          |  /    |  / 
          | /     | /
          |/      |/
   (x,y,z)o-------+
         /      |
     Back       Below
         
          |<----->|
             size
*/                                  
void Chunk::addFace(int x, int y, int z, facePos facing, int size)
{   
   vertex v;
   int s = size;
   v.w = y; // Vector.w is static.
   
   switch (facing) 
   {
   case ABOVE:
      // Add a new face.
      v.x = x+s; v.y = y+s; v.z = z+s; verticesAboveBuf.push_back(v); //C 
      v.x = x+s; v.y = y+s; v.z = z;   verticesAboveBuf.push_back(v); //D
      v.x = x;   v.y = y+s; v.z = z+s; verticesAboveBuf.push_back(v); //B
      v.x = x;   v.y = y+s; v.z = z+s; verticesAboveBuf.push_back(v); //B 
      v.x = x+s; v.y = y+s; v.z = z;   verticesAboveBuf.push_back(v); //D
      v.x = x;   v.y = y+s; v.z = z;   verticesAboveBuf.push_back(v); //A
      break;
   case BELOW:
      // Add a new face.
      v.x = x+s; v.y = y;   v.z = z+s; verticesBelowBuf.push_back(v); //C
      v.x = x;   v.y = y;   v.z = z+s; verticesBelowBuf.push_back(v); //B
      v.x = x+s; v.y = y;   v.z = z;   verticesBelowBuf.push_back(v); //D
      v.x = x+s; v.y = y;   v.z = z;   verticesBelowBuf.push_back(v); //D
      v.x = x;   v.y = y;   v.z = z+s; verticesBelowBuf.push_back(v); //B
      v.x = x;   v.y = y;   v.z = z;   verticesBelowBuf.push_back(v); //A
      break;
   case LEFT:
      // Add a new face.
      v.x = x;   v.y = y+s; v.z = z+s; verticesLeftBuf.push_back(v); //A
      v.x = x;   v.y = y+s; v.z = z;   verticesLeftBuf.push_back(v); //B
      v.x = x;   v.y = y;   v.z = z+s; verticesLeftBuf.push_back(v); //D
      v.x = x;   v.y = y;   v.z = z+s; verticesLeftBuf.push_back(v); //D
      v.x = x;   v.y = y+s; v.z = z;   verticesLeftBuf.push_back(v); //B
      v.x = x;   v.y = y;   v.z = z;   verticesLeftBuf.push_back(v); //C
      break;
   case RIGHT:
      // Add a new face.
      v.x = x+s; v.y = y+s; v.z = z+s; verticesRightBuf.push_back(v); //B
      v.x = x+s; v.y = y;   v.z = z+s; verticesRightBuf.push_back(v); //C
      v.x = x+s; v.y = y+s; v.z = z;   verticesRightBuf.push_back(v); //A
      v.x = x+s; v.y = y+s; v.z = z;   verticesRightBuf.push_back(v); //A
      v.x = x+s; v.y = y;   v.z = z+s; verticesRightBuf.push_back(v); //C
      v.x = x+s; v.y = y;   v.z = z;   verticesRightBuf.push_back(v); //D
      break;
   case FRONT:
      // Add a new face.
      v.x = x+s; v.y = y+s; v.z = z+s; verticesFrontBuf.push_back(v); //A
      v.x = x;   v.y = y+s; v.z = z+s; verticesFrontBuf.push_back(v); //B
      v.x = x;   v.y = y;   v.z = z+s; verticesFrontBuf.push_back(v); //C
      v.x = x;   v.y = y;   v.z = z+s; verticesFrontBuf.push_back(v); //C
      v.x = x+s; v.y = y;   v.z = z+s; verticesFrontBuf.push_back(v); //D
      v.x = x+s; v.y = y+s; v.z = z+s; verticesFrontBuf.push_back(v); //A
      break;
   case BACK:
      // Add a new face.
      v.x = x;   v.y = y;   v.z = z;   verticesBackBuf.push_back(v); //D
      v.x = x;   v.y = y+s; v.z = z;   verticesBackBuf.push_back(v); //A
      v.x = x+s; v.y = y+s; v.z = z;   verticesBackBuf.push_back(v); //B
      v.x = x+s; v.y = y+s; v.z = z;   verticesBackBuf.push_back(v); //B
      v.x = x+s; v.y = y;   v.z = z;   verticesBackBuf.push_back(v); //C
      v.x = x;   v.y = y;   v.z = z;   verticesBackBuf.push_back(v); //D
      break;
   default:
      break;
   }
}             

void Chunk::setLeftNeighbour(Chunk* neighbour)
{
   leftNeighbour = neighbour;
}

void Chunk::setRightNeighbour(Chunk* neighbour)
{
   rightNeighbour = neighbour;
}

void Chunk::setFrontNeighbour(Chunk* neighbour)
{
   frontNeighbour = neighbour;
}

void Chunk::setBackNeighbour(Chunk* neighbour)
{
   backNeighbour = neighbour;
}

void Chunk::setTopNeighbour(Chunk* neighbour)
{
   topNeighbour = neighbour;
}

void Chunk::setBottomNeighbour(Chunk* neighbour)
{
   bottomNeighbour = neighbour;
}

bool Chunk::rightBorderFull()
{
   return chunkData.rightBorderFull();
}

bool Chunk::leftBorderFull()
{
   return chunkData.leftBorderFull();
}

bool Chunk::topBorderFull()
{
   return chunkData.topBorderFull();
}

bool Chunk::bottomBorderFull()
{
   return chunkData.bottomBorderFull();
}

bool Chunk::frontBorderFull()
{
   return chunkData.frontBorderFull();
}

bool Chunk::backBorderFull()
{
   return chunkData.backBorderFull();
}

// Return true if this chunk is obscured by its neighbours.
bool Chunk::obscuredByChunkNeighbours()
{
   // If any neighbours have null pointers then this chunk must be visible.
   // (edge of world).
   if(leftNeighbour == 0 or
      rightNeighbour == 0 or
      topNeighbour == 0 or
      bottomNeighbour == 0 or
      frontNeighbour == 0 or
      backNeighbour == 0)
   {
      return false;
   }
   else
   {
      if(leftNeighbour->rightBorderFull() and
         rightNeighbour->leftBorderFull() and
         topNeighbour->bottomBorderFull() and
         bottomNeighbour->topBorderFull() and
         backNeighbour->frontBorderFull() and
         frontNeighbour->backBorderFull())
      {
         return true;
      }
      return false;
   }
}

bool Chunk::leftNeighbourIsSolid(int x, int y, int z)
{
   if(leftNeighbour)
   {
      return leftNeighbour->is_solid(x,y,z);
   }
   return false;
}

bool Chunk::rightNeighbourIsSolid(int x, int y, int z)
{
   if(rightNeighbour)
   {
      return rightNeighbour->is_solid(x,y,z);
   }
   return false;
}

bool Chunk::frontNeighbourIsSolid(int x, int y, int z)
{
   if(frontNeighbour)
   {
      return frontNeighbour->is_solid(x,y,z);
   }
   return false;
}

bool Chunk::backNeighbourIsSolid(int x, int y, int z)
{
   if(backNeighbour)
   {
      return backNeighbour->is_solid(x,y,z);
   }
   return false;
}

bool Chunk::topNeighbourIsSolid(int x, int y, int z)
{
   if(topNeighbour)
   {
      return topNeighbour->is_solid(x,y,z);
   }
   return false;
}

bool Chunk::bottomNeighbourIsSolid(int x, int y, int z)
{
   if(bottomNeighbour)
   {
      return bottomNeighbour->is_solid(x,y,z);
   }
   return false;
}

bool Chunk::is_solid(int x, int y, int z)
{
   return chunkData.is_solid(x,y,z);
}

bool Chunk::is_solid(vector3i index)
{
   return chunkData.is_solid(index.x, index.y, index.z);
}