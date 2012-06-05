   // Successive Reverse Subdivision Strategy
   // Take 2D 'slices' of the volume, and iterate across them with a 2x2 window
   // to construct a lower resolution map of windows with values 0 to 16.
   // Repeat by merging neighbouring windows where possible.
   for(z = 0; z < dim.z; z++)
   {
      for(y = 0; y < (dim.y - 2); y += 2)
      {
         for(x = 0; x < (dim.x - 2); x += 2)
         {
            // Determine the configuration of this window.
            // (Bools are TRUE if that cell is drawable and requires vertices)
            window = 0;
            window += (data.is_solid(x,y,z) and !data.is_solid(x,y,z-1))?1:0;
            window += (data.is_solid(x+1,y,z) and !data.is_solid(x+1,y,z-1))?2:0;
            window += (data.is_solid(x,y+1,z) and !data.is_solid(x,y+1,z-1))?4:0;
            window += (data.is_solid(x+1,y+1,z) and !data.is_solid(x+1,y+1,z-1))?8:0;
            windows[Position] = window;
         }
      }
   }
   
   for(std::map<Position, byte>::iterator ii=data.data.begin(); ii!=data.data.end(); ++ii)
   {        
      // Get the window coords.
      x = (*ii).first.tuple.get<0>();
      y = (*ii).first.tuple.get<1>();
      z = (*ii).first.tuple.get<2>();
   
   
   
   
            
            // This should be implemented as a LUT
            if(cell_0_0 and cell_1_0 and cell_0_1 and cell_1_1)
            {
               // All cells are drawable, we need one big quad.
               // Maybe add this to a new list for further processing later?
               // Neighbouring large quads can be merged further.
               // Add a new face.
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            else if(!cell_0_0 and cell_0_1 and cell_1_0 and cell_1_1)
            {
               // Upper Right Cell
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Lower Left Cell
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Lower Right Cell
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            else if(cell_0_0 and !cell_0_1 and cell_1_0 and cell_1_1)
            {
               // Upper Left Cell
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Upper Right Cell
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Lower Right Cell
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            else if(cell_0_0 and cell_0_1 and !cell_1_0 and cell_1_1)
            {
               // Upper Left Cell
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Lower Left Cell
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Lower Right Cell
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            else if(cell_0_0 and cell_0_1 and cell_1_0 and !cell_1_1)
            {
               // Upper Left Cell
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Upper Right Cell
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Lower Left Cell
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            // Now cover the two diagonal cases:
            else if(!cell_0_0 and cell_0_1 and cell_1_0 and !cell_1_1)
            {
               // Upper Right Cell
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Lower Left Cell
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            else if(cell_0_0 and !cell_0_1 and !cell_1_0 and cell_1_1)
            {
               // Upper Left Cell
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Lower Right Cell
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            // Now cover the cases where a rectangle is needed (4 cases).
            else if(!cell_0_0 and !cell_0_1 and cell_1_0 and cell_1_1)
            {
               // Right Side Vertical Rect
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            else if(!cell_0_0 and cell_0_1 and !cell_1_0 and cell_1_1)
            {
               // Top Side Horizontal Rect
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            else if(cell_0_0 and !cell_0_1 and cell_1_0 and !cell_1_1)
            {
               // Bottom Side Horizontal Rect
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            else if(cell_0_0 and cell_0_1 and !cell_1_0 and !cell_1_1)
            {
               // Left Side Vertical Rect
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            // Now check for individual drawable cells, only one will be visible
            else
            {
               if(cell_0_0)
               {
                  // Upper Left Cell
                  vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
                  vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
                  vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
                  vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
                  vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
                  vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               }
               if(cell_1_0)
               {
                  // Upper Right Cell
                  vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
                  vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
                  vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
                  vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
                  vert.x = x+2; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
                  vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               }
               if(cell_0_1)
               {
                  // Lower Left Cell
                  vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
                  vert.x = x;   vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
                  vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
                  vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
                  vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
                  vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               }
               if(cell_1_1)
               {
                  // Lower Right Cell
                  vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
                  vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
                  vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
                  vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
                  vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
                  vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               }
            }
         }
      }
   }

   // Quad Looping Strategy
   // Take 2D 'slices' of the volume, and iterate across them with a 2x2 window.
   for(z = 0; z < dim.z; z++)
   {
      for(y = 0; y < (dim.y - 2); y += 2)
      {
         for(x = 0; x < (dim.x - 2); x += 2)
         {
            // Determine the configuration of this window.
            // (Bools are TRUE if that cell is drawable and requires vertices)
            cell_0_0 = data.is_solid(x,y,z) and !data.is_solid(x,y,z-1);
            cell_1_0 = data.is_solid(x+1,y,z) and !data.is_solid(x+1,y,z-1);
            cell_0_1 = data.is_solid(x,y+1,z) and !data.is_solid(x,y+1,z-1);
            cell_1_1 = data.is_solid(x+1,y+1,z) and !data.is_solid(x+1,y+1,z-1);
            
            // This should be implemented as a LUT
            if(cell_0_0 and cell_1_0 and cell_0_1 and cell_1_1)
            {
               // All cells are drawable, we need one big quad.
               // Maybe add this to a new list for further processing later?
               // Neighbouring large quads can be merged further.
               // Add a new face.
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            else if(!cell_0_0 and cell_0_1 and cell_1_0 and cell_1_1)
            {
               // Upper Right Cell
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Lower Left Cell
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Lower Right Cell
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            else if(cell_0_0 and !cell_0_1 and cell_1_0 and cell_1_1)
            {
               // Upper Left Cell
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Upper Right Cell
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Lower Right Cell
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            else if(cell_0_0 and cell_0_1 and !cell_1_0 and cell_1_1)
            {
               // Upper Left Cell
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Lower Left Cell
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Lower Right Cell
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            else if(cell_0_0 and cell_0_1 and cell_1_0 and !cell_1_1)
            {
               // Upper Left Cell
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Upper Right Cell
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Lower Left Cell
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            // Now cover the two diagonal cases:
            else if(!cell_0_0 and cell_0_1 and cell_1_0 and !cell_1_1)
            {
               // Upper Right Cell
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Lower Left Cell
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            else if(cell_0_0 and !cell_0_1 and !cell_1_0 and cell_1_1)
            {
               // Upper Left Cell
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               // Lower Right Cell
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            // Now cover the cases where a rectangle is needed (4 cases).
            else if(!cell_0_0 and !cell_0_1 and cell_1_0 and cell_1_1)
            {
               // Right Side Vertical Rect
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            else if(!cell_0_0 and cell_0_1 and !cell_1_0 and cell_1_1)
            {
               // Top Side Horizontal Rect
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            else if(cell_0_0 and !cell_0_1 and cell_1_0 and !cell_1_1)
            {
               // Bottom Side Horizontal Rect
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            else if(cell_0_0 and cell_0_1 and !cell_1_0 and !cell_1_1)
            {
               // Left Side Vertical Rect
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               vert.x = x;   vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
               vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
               vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
            }
            // Now check for individual drawable cells, only one will be visible
            else
            {
               if(cell_0_0)
               {
                  // Upper Left Cell
                  vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
                  vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
                  vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
                  vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
                  vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
                  vert.x = x;   vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               }
               if(cell_1_0)
               {
                  // Upper Right Cell
                  vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
                  vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
                  vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
                  vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
                  vert.x = x+2; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
                  vert.x = x+1; vert.y = y;   vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               }
               if(cell_0_1)
               {
                  // Lower Left Cell
                  vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
                  vert.x = x;   vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
                  vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
                  vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
                  vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
                  vert.x = x;   vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               }
               if(cell_1_1)
               {
                  // Lower Right Cell
                  vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
                  vert.x = x+1; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //A
                  vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
                  vert.x = x+2; vert.y = y+2; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //B
                  vert.x = x+2; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //C
                  vert.x = x+1; vert.y = y+1; vert.z = z;   vert.w = y; verticesBack.push_back(vert); //D
               }
            }
         }
      }
   }