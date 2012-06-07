import: constants

float getHelixChannel(float x, float p0, float p1)
{
   // Gamma factor to emphasise either low intensity values (gamma < 1)
   // or high intensity values (gamma > 1)
   float gamma = 1.0;
   // Start colour (purple)
   float s = 0.5;
   // Number of r,g,b rotations in colour that are made from start to end.
   float r = -1.5;
   // Hue (colour saturation) 0 = greyscale.
   float h = 1.0;
   // Apply gamma factor to emphasise low or high intensity values
   float xg = pow(x,gamma);
   
   // Calculate amplitude and angle of deviation from the black
   // to white diagonal in the plane of constant perceived intensity
   float a = h * xg * (1 - xg) / 2;
   float phi = 2 * pi * (s / 3 + r * x);
   
   return xg + a * (p0 * cos(phi) + p1 * sin(phi));
}

vec3 cubeHelix(float x)
{
   // Implement D.A. Green's cubehelix colour scheme
   
   // Input X ranges from 0 to 1.

   float red, green, blue;

   red = getHelixChannel(x, -0.14861, 1.78277);
   green = getHelixChannel(x, -0.29227, -0.90649);
   blue = getHelixChannel(x, -1.97294, 0.0);
   
   return vec3(red, green, blue);
}