#version 400

vertex:
   in vec4 position;                // The vertex positions within the chunk.
   uniform vec3 normal;
   uniform vec3 worldPosition;      // The base position of the chunk in the world.
   uniform mat4 mvp;                // Model-view projection matrix.
   uniform int worldHeight;
   //uniform mat4 modelview;
   
   varying float texcoord;
   
   out Data{
      vec3 fnormal;
      float depth;
   } data;
      
   void main(void)
   {  
      const vec4 vectorOffset = vec4(worldPosition, 0.0);
      vec4 vertex = vec4(position.x, position.y, position.z, 1);
      data.fnormal = normal;
      data.depth = 1-(min(1,length((mvp * (vertex + vectorOffset)).xyz)/(worldHeight/2)));
      texcoord = mod(float(position.w + worldPosition.y) / worldHeight,1);
      gl_Position = mvp * (vertex + vectorOffset);
   }

fragment:
   
   import: colourmaps
   import: utilities

   out vec3 fragment;
   
   varying float texcoord;
   
   in Data{
      vec3 fnormal;
      float depth;
   } data;
   
   void main()
   {   
      float red, green, blue;
      vec3 colours;
      
      colours = thermal(data.depth);
      red = colours.x;
      //red = 1.0f;
      green = colours.y;
      //green = 1.0f;
      blue = colours.z;
      //blue = 1.0f;
      
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
      //fragment = fog(vec3(red, green, blue),vec3(0.95, 0.95, 0.95),data.depth,0.003);
      fragment = vec3(red, green, blue);
   }