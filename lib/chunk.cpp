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

Chunk::Chunk(int chunk_x, int chunk_y, int chunk_z, int chunk_size, int wh) : chunkData(chunk_size)
{
   worldHeight = wh;
   dim.x = chunk_size;
   dim.y = chunk_size;
   dim.z = chunk_size;
   pos.x = chunk_x;
   pos.y = chunk_y;
   pos.z = chunk_z;
   centre.x = pos.x + 0.5 * dim.x;
   centre.y = pos.y + 0.5 * dim.y;
   centre.z = pos.z + 0.5 * dim.z;
   
   visibleFaceGroups.x = DRAW_BOTH_X;
   visibleFaceGroups.y = DRAW_BOTH_Y;
   visibleFaceGroups.z = DRAW_BOTH_Z;
   
   std::cout << "CPP: Generating chunk with position " << pos.x << "," << pos.y << "," << pos.z << std::endl;
   size = chunk_size;
   modified = false;
   visible = false;
   meshBuildInProgress = false;
   voxels = 0;
   // Init VBO
   glGenBuffers(1, &verticesFrontVBO);
   glGenBuffers(1, &verticesBackVBO);
   glGenBuffers(1, &verticesLeftVBO);
   glGenBuffers(1, &verticesRightVBO);
   glGenBuffers(1, &verticesAboveVBO);
   glGenBuffers(1, &verticesBelowVBO);
   
   firstDrawCall = true;
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
   centre.x = pos.x + 0.5 * dim.x;
   centre.y = pos.y + 0.5 * dim.y;
   centre.z = pos.z + 0.5 * dim.z;
}

glm::vec3 Chunk::position()
{
   return pos;
}

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
   else if(cam.x > (pos.x + dim.x))
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
   else if(cam.y > (pos.y + dim.y))
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
   else if(cam.z > (pos.z + dim.z))
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

void Chunk::buildDisplayList()
{
   //
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

unsigned int Chunk::voxelcount()
{
   return voxels;
}

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
      if(useFastMeshBuilder)
      {
         workDone = meshBuilderFast();
      }
      else
      {
         meshBuilderSlow();
      }
      // If the update completed this cycle rebuild the display list.
      if(!meshBuildInProgress)
      {
         buildDisplayList();
      }
   }
   return workDone;
   //std::cout << "CPP: Chunk mesh rebuild...(" << GetTimeMs64() - time << "ms)" << std::endl;
}

void Chunk::initDraw(GLuint program)
{   
	glUseProgram(program);
   posAttrib = glGetAttribLocation(program, "position");
   normAttrib = glGetUniformLocation(program, "normal");
   worldPosAttrib = glGetUniformLocation(program, "worldPosition");
   worldHeightAttrib = glGetUniformLocation(program, "worldHeight");
	glUseProgram(0);
}

void Chunk::draw(GLuint program, 
                 glm::vec3 camPosition, 
                 glm::mat4 mvp, 
                 int viewDistance)
{
   // If there is nothing to draw then abort early.
   if(!visible)
   {
      return;
   }
   // Set up the vertex buffers if this is the first draw call.
   if(firstDrawCall)
   {
      firstDrawCall = false;
      initDraw(program);
   }
   
   /* Frustrum Culling */
   // http://en.wikibooks.org/wiki/OpenGL_Programming/Glescraft_5
   
   // Determine the clip space coords of the chunk centre.
   glm::vec4 coords = mvp * glm::vec4(centre, 1);
   coords.x /= coords.w;
   coords.y /= coords.w;
   
   // Diameter of bounding sphere holding the chunk.
   float diameter = sqrtf(centre.x*centre.x + centre.y*centre.y + centre.z*centre.z);
   // How far away the chunk is from the camera.
   float distance = glm::length(coords);
   
   // If the chunk has a negative Z in clip space then it is behind the camera.
   if(coords.z < -diameter)
   {
      verticesRenderedCount = 0;
      return;
   }
   
   // If the chunk is out of the viewing distance dont draw it.
   if(distance > viewDistance)
   {
      verticesRenderedCount = 0;
      return;
   }
   
   // If the chunk is outside the view frustrum plus a tolerance then dont draw.
   diameter /= fabsf(coords.w);   
   if(fabsf(coords.x) > 1 + diameter or fabsf(coords.y > 1 + diameter))
   {
      verticesRenderedCount = 0;
      return;
   }
      
	glUseProgram(program);
   setVisibleFaceGroup(camPosition);
         
   // Tell GLSL the world coords of this chunk. 
   glUniform3fv(worldPosAttrib, 1, glm::value_ptr(pos));
   // Tell GLSL the world height in voxels. 
   glUniform1i(worldHeightAttrib, worldHeight);
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
  
void Chunk::fill()
{
   chunkData.fill();
   modified = true;
   visible = true;
}

void Chunk::empty()
{
   chunkData.empty();
   modified = true;
   visible = false;
}

void Chunk::load(byte* initialiser, int chunk_size)
{
   int x,y,z;
   if (chunk_size != size)
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
               chunkData.set(x,y,z,initialiser[x + (y * size) + (z * size * size)]);
            }
         }
      }
   }
   modified = chunkData.is_modified();
   visible = !chunkData.is_empty();
   chunkData.clearModifiedState();
}

void Chunk::set(int x, int y, int z, byte initialiser)
{
   chunkData.set(x,y,z,initialiser);
   modified = chunkData.is_modified() or modified;
   visible = !chunkData.is_empty();
   chunkData.clearModifiedState();
}

// Create a pillar of voxels at x,z with height h.
void Chunk::setHeight(int x, int z, int h)
{
   int y;
   // Clamp height values.
   h = h >= dim.y ? dim.y - 1 : h;
   h = h < 0 ? 0 : h;
   chunkData.yRangeSet(x,0,h,z,1);
   modified = chunkData.is_modified() or modified;
   visible = !chunkData.is_empty();
   chunkData.clearModifiedState();
}

byte Chunk::get(int x, int y, int z)
{
   return chunkData.get(x,y,z);
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

bool Chunk::requireMeshUpdate()
{
   return not meshBuildInProgress and modified and visible;
}

bool Chunk::meshBuildRunning()
{
   return meshBuildInProgress;
}

// Generate a triangle mesh which can be used to represent
// the voxel volume as a regular grid of cubes.
void Chunk::meshBuilderSlow()
{
   int x,y,z,i;              // Chunk x,y,z
       
   verticesLeft.clear();   // Clear any previous mesh data.
   verticesRight.clear();
   verticesAbove.clear();
   verticesBelow.clear();
   verticesFront.clear();
   verticesBack.clear();
   
   int boundXMax, boundXMin;
   int boundYMax, boundYMin;
   int boundZMin, boundZMax;
   int axis1, axis2, axis3;
   int major_axis;
   int sliceX, sliceY;
   int rectX,rectY;
   bool extend_x, extend_y;
   bool tracing;
   const facePos AXIS[] = {BACK, FRONT, RIGHT, LEFT, BELOW, ABOVE};
   facePos facing;
   
   byte marker = 0;
   
   std::map<std::pair<int,int>, int> vismap;   
            
   // Loop through every voxel in the volume and create the vertices required
   // to render exposed faces. Faces hidden by surrounding cubes are not drawn.
   for(i = 0; i < 6; i++)
   {
      facing = AXIS[i];
      for(major_axis = 0; major_axis < dim.z; major_axis++)
      {
         // Take successive 2D slices of the volume data over the major axis.
         // Build a visibility map for the mesh builder to use.
         vismap.clear();
         switch (facing)
         {
            case ABOVE:
               for(sliceY = 0; sliceY < dim.y; sliceY++)
               {
                  for(sliceX = 0; sliceX < dim.x; sliceX++)
                  {
                     // Check visibility.
                     if(chunkData.is_solid(sliceX,major_axis,sliceY) and chunkData.blockAboveVisible(sliceX,major_axis,sliceY))
                     {
                        // Mark this cell as visible. The mesh gen will draw this cell
                        vismap[std::make_pair(sliceX,sliceY)] = 1;
                     }
                  }
               }
               break;
            case BELOW:
               for(sliceY = 0; sliceY < dim.y; sliceY++)
               {
                  for(sliceX = 0; sliceX < dim.x; sliceX++)
                  {
                     // Check visibility.
                     if(chunkData.is_solid(sliceX,major_axis,sliceY) and chunkData.blockBelowVisible(sliceX,major_axis,sliceY))
                     {
                        // Mark this cell as visible. The mesh gen will draw this cell
                        vismap[std::make_pair(sliceX,sliceY)] = 1;
                     }
                  }
               }
               break;
            case LEFT:
               for(sliceY = 0; sliceY < dim.y; sliceY++)
               {
                  for(sliceX = 0; sliceX < dim.x; sliceX++)
                  {
                     // Check visibility.
                     if(chunkData.is_solid(major_axis,sliceY,sliceX) and chunkData.blockLeftVisible(major_axis,sliceY,sliceX))
                     {
                        // Mark this cell as visible. The mesh gen will draw this cell
                        vismap[std::make_pair(sliceX,sliceY)] = 1;
                     }
                  }
               }
               break;
            case RIGHT:
               for(sliceY = 0; sliceY < dim.y; sliceY++)
               {
                  for(sliceX = 0; sliceX < dim.x; sliceX++)
                  {
                     // Check visibility.
                     if(chunkData.is_solid(major_axis,sliceY,sliceX) and chunkData.blockRightVisible(major_axis,sliceY,sliceX))
                     {
                        // Mark this cell as visible. The mesh gen will draw this cell
                        vismap[std::make_pair(sliceX,sliceY)] = 1;
                     }
                  }
               }
               break;
            case FRONT:
               for(sliceY = 0; sliceY < dim.y; sliceY++)
               {
                  for(sliceX = 0; sliceX < dim.x; sliceX++)
                  {
                     // Check visibility.
                     if(chunkData.is_solid(sliceX,sliceY,major_axis) and chunkData.blockFrontVisible(sliceX,sliceY,major_axis))
                     {
                        // Mark this cell as visible. The mesh gen will draw this cell
                        vismap[std::make_pair(sliceX,sliceY)] = 1;
                     }
                  }
               }
               break;
            case BACK:
               for(sliceY = 0; sliceY < dim.y; sliceY++)
               {
                  for(sliceX = 0; sliceX < dim.x; sliceX++)
                  {
                     // Check visibility.
                     if(chunkData.is_solid(sliceX,sliceY,major_axis) and chunkData.blockBackVisible(sliceX,sliceY,major_axis))
                     {
                        // Mark this cell as visible. The mesh gen will draw this cell
                        vismap[std::make_pair(sliceX,sliceY)] = 1;
                     }
                  }
               }
               break;
            default:
               break;
         }
         // Now use the visibility map to create a mesh for this slice.
         sliceX = 0;
         sliceY = 0;
         marker = 0;
         tracing = false;
         while ((sliceX < dim.x) or (sliceY < dim.y))
         {
            if (!tracing)
            {
               if (vismap[std::make_pair(sliceX,sliceY)] == 1)
               {
                  tracing = true;
                  // Add the top left bounds of this rectangle.
                  boundXMin = sliceX;
                  boundYMin = sliceY;
                  marker++;
               }
               else
               {
                  // Move to the next cell.
                  sliceX = sliceX < dim.x?sliceX+1:0;
                  sliceY = sliceX < dim.x?sliceY:sliceY+1;
               }
            }
            else
            {
               extend_x = extend_y = false;
               // Determine in which axis the rectangle can be expanded. 
               // If we have expanded more in one axis than the other, prevent 
               // further expansions in the lesser axis.
               if((sliceY - boundYMin) >= (sliceX - boundXMin) and sliceY < dim.y)
               {
                  extend_y = true;
                  for(rectX = boundXMin; rectX < sliceX+1; rectX++)
                  {
                     if (vismap[std::make_pair(rectX,sliceY+1)] != 1)
                     {
                        extend_y = false;
                        break;
                     }
                  }
               }
               if((sliceX - boundXMin) >= (sliceY - boundYMin) and sliceX < dim.x)
               {
                  extend_x = true;
                  for(rectY = boundYMin; rectY < sliceY+1; rectY++)
                  {
                     if (vismap[std::make_pair(sliceX+1,rectY)] != 1)
                     {
                        extend_x = false;
                        break;
                     }
                  }
               }
               // Expand the rectangle if possible to do so.
               if (extend_x and extend_y)
               {
                  sliceX++;
                  sliceY++;
               }
               else if (extend_x)
               {
                  sliceX++;
               }
               else if (extend_y)
               {
                  sliceY++;
               }
               else
               {
                  // Further expansion is not possible. 
                  boundXMax = sliceX;
                  boundYMax = sliceY;
                  boundZMin = boundZMax = major_axis;
                  // Mark all cells encompassed by this rectangle as 'drawn':
                  for(rectY = boundYMin; rectY < boundYMax + 1; rectY++)
                  {
                     for(rectX = boundXMin; rectX < boundXMax + 1; rectX++)
                     {
                        vismap.erase(std::make_pair(rectX,rectY));
                     }
                  }
                  // Add the vertices required to draw this rectangle:
                  addFace2( boundXMin, boundXMax, 
                           boundYMin, boundYMax,
                           boundZMin, boundZMax, 
                           facing, marker);
                  // Move sliceX and sliceY to top right of the rectangle:
                  sliceY = boundXMax+1<dim.x?boundYMin:boundYMin+1;
                  sliceX = boundXMax+1<dim.x?boundXMax+1:0;
                  // Turn off tracing:
                  tracing = false;
               }
            }
         }
      }
   }
}

// Generate a triangle mesh which can be used to represent
// the voxel volume as a regular grid of cubes.
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
      // First check to see if the chunk is completely full.
      if(chunkData.is_full())
      {
         // The chunk is full, create a primitive mesh and terminate mesh build.
         addFace(0, 0, 0, LEFT, size);
         addFace(0, 0, 0, RIGHT, size);
         addFace(0, 0, 0, ABOVE, size);
         addFace(0, 0, 0, BELOW, size);
         addFace(0, 0, 0, BACK, size);
         addFace(0, 0, 0, FRONT, size);
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
      if(chunkData.blockLeftVisible(x,y,z))
      {
         addFace(x, y, z, LEFT, 1);
      }
      
      if(chunkData.blockRightVisible(x,y,z))
      {
         addFace(x, y, z, RIGHT, 1);
      }
      
      if(chunkData.blockBelowVisible(x,y,z))
      {
         addFace(x, y, z, BELOW, 1);
      }
            
      if(chunkData.blockAboveVisible(x,y,z))
      {
         addFace(x, y, z, ABOVE, 1);
      }
      
      if(chunkData.blockBackVisible(x,y,z))
      {
         addFace(x, y, z, BACK, 1);
      }
      
      if(chunkData.blockFrontVisible(x,y,z))
      {
         addFace(x, y, z, FRONT, 1);
      }
      ++ii;
   }
   return meshGenWorkAllowance;
}

void Chunk::addFace2(int xmin, int xmax,
                        int ymin, int ymax,
                        int zmin, int zmax,
                        facePos facing, byte value
                       )
{
   vertex vert;
   switch (facing) 
   {
   case ABOVE:
      // Add a new face.
      std::swap(ymin,zmin); // Swap Y and Z for this axis configuration.
      std::swap(ymax,zmax);
      vert.y = ymax+1;
      vert.w = value;
      vert.x = xmax+1; vert.z = zmax+1; verticesAbove.push_back(vert); //B
      vert.x = xmax+1; vert.z = zmin;   verticesAbove.push_back(vert); //C
      vert.x = xmin;   vert.z = zmax+1; verticesAbove.push_back(vert); //A
      vert.x = xmin;   vert.z = zmax+1; verticesAbove.push_back(vert); //A
      vert.x = xmax+1; vert.z = zmin;   verticesAbove.push_back(vert); //C
      vert.x = xmin;   vert.z = zmin;   verticesAbove.push_back(vert); //D
      break;
   case BELOW:
      // Add a new face.
      std::swap(ymin,zmin); // Swap Y and Z for this axis configuration.
      std::swap(ymax,zmax);
      vert.y = ymin;
      vert.w = value;
      vert.x = xmax+1; vert.z = zmax+1; verticesBelow.push_back(vert); //B
      vert.x = xmin;   vert.z = zmax+1; verticesBelow.push_back(vert); //C
      vert.x = xmax+1; vert.z = zmin;   verticesBelow.push_back(vert); //A
      vert.x = xmax+1; vert.z = zmin;   verticesBelow.push_back(vert); //A
      vert.x = xmin;   vert.z = zmax+1; verticesBelow.push_back(vert); //C
      vert.x = xmin;   vert.z = zmin;   verticesBelow.push_back(vert); //D
      break;
   case LEFT:
      // Add a new face.
      std::swap(xmin,zmin); // Swap X and Z for this axis configuration.
      std::swap(xmax,zmax);
      vert.x = xmin;
      vert.w = value;
      vert.y = ymax+1; vert.z = zmax+1; verticesLeft.push_back(vert); //B
      vert.y = ymax+1; vert.z = zmin;   verticesLeft.push_back(vert); //C
      vert.y = ymin;   vert.z = zmax+1; verticesLeft.push_back(vert); //A
      vert.y = ymin;   vert.z = zmax+1; verticesLeft.push_back(vert); //A
      vert.y = ymax+1; vert.z = zmin;   verticesLeft.push_back(vert); //C
      vert.y = ymin;   vert.z = zmin;   verticesLeft.push_back(vert); //D
      break;
   case RIGHT:
      // Add a new face.
      std::swap(xmin,zmin); // Swap X and Z for this axis configuration.
      std::swap(xmax,zmax);
      vert.x = xmax+1;
      vert.w = value;
      vert.y = ymax+1; vert.z = zmax+1; verticesRight.push_back(vert); //B
      vert.y = ymin;   vert.z = zmax+1; verticesRight.push_back(vert); //C
      vert.y = ymax+1; vert.z = zmin;   verticesRight.push_back(vert); //A
      vert.y = ymax+1; vert.z = zmin;   verticesRight.push_back(vert); //A
      vert.y = ymin;   vert.z = zmax+1; verticesRight.push_back(vert); //C
      vert.y = ymin;   vert.z = zmin;   verticesRight.push_back(vert); //D
      break;
   case FRONT:
      // Add a new face.
      vert.z = zmax+1;
      vert.w = value;
      vert.x = xmax+1; vert.y = ymax+1; verticesFront.push_back(vert); //D
      vert.x = xmin;   vert.y = ymax+1; verticesFront.push_back(vert); //A
      vert.x = xmin;   vert.y = ymin;   verticesFront.push_back(vert); //B
      vert.x = xmin;   vert.y = ymin;   verticesFront.push_back(vert); //B
      vert.x = xmax+1; vert.y = ymin;   verticesFront.push_back(vert); //C
      vert.x = xmax+1; vert.y = ymax+1; verticesFront.push_back(vert); //D
      break;
   case BACK:
      // Add a new face.
      vert.z = zmin;
      vert.w = value;
      vert.x = xmin;   vert.y = ymin;   verticesBack.push_back(vert); //D
      vert.x = xmin;   vert.y = ymax+1; verticesBack.push_back(vert); //A
      vert.x = xmax+1; vert.y = ymax+1; verticesBack.push_back(vert); //B
      vert.x = xmax+1; vert.y = ymax+1; verticesBack.push_back(vert); //B
      vert.x = xmax+1; vert.y = ymin;   verticesBack.push_back(vert); //C
      vert.x = xmin;   vert.y = ymin;   verticesBack.push_back(vert); //D
      break;
   default:
      break;
   }
}
                                       
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