#version 400

vertex:
   in vec4 position;                // The vertex positions within the chunk.
   uniform vec3 normal;
   uniform vec3 worldPosition;      // The base position of the chunk in the world.
   uniform mat4 mvp;                // Model-view projection matrix.
   //uniform mat4 modelview;
   
   varying int texcoord;
   
   out Data{
      vec3 fnormal;
   } data;
      
   void main(void)
   {  
      const vec4 vectorOffset = vec4(worldPosition, 0.0);
      vec4 vertex = vec4(position.x, position.y, position.z, 1);
      data.fnormal = normal;
      // Depth = length((modelview * (vertex + vectorOffset)).xyz);
      texcoord = int(position.w + worldPosition.y);
      gl_Position = mvp * (vertex + vectorOffset);
   }

fragment:
   
   import: colourmaps

   out vec3 fragment;
   
   varying int texcoord;
   
   in Data{
      vec3 fnormal;
   } data;
   
   void main()
   {
      // Jet Colourmap      
      float k = 4*(texcoord/float(255));
      
      float red, green, blue;
      vec3 colours;
      
      colours = cubeHelix(k);
      red = colours.x;
      green = colours.y;
      blue = colours.z;
      
      // Attenuate facecolour for each normal to accentuate the cubes.
      if(data.fnormal.y == 0)
      {
         red = red * 0.95;
         green = green * 0.95;
         blue = blue * 0.95;
         if(data.fnormal.x == 0)
         {
            red = red * 0.95;
            green = green * 0.95;
            blue = blue * 0.95;
            if(data.fnormal.z == 0)
            {
               red = red * 0.95;
               green = green * 0.95;
               blue = blue * 0.95;
            }
         }
      }
      fragment = vec3(red, green, blue);
   }