////////////////////////////////////////////////////////////////////////////////
// Small Volume   A utility class for storing and accessing chunk data.     
//                                                                        
// @author         Peter A Bennett
// @copyright      (c) 2012 Peter A Bennett
// @license        LGPL      
// @email          pab850@googlemail.com
// @contact        www.bytebash.com
//
////////////////////////////////////////////////////////////////////////////////

#include "smallVolume.hpp"

bool operator==(const vector4f &a, const vector4f &b)
{
   return a.x == b.x &&
          a.y == b.y &&
          a.z == b.z &&
          a.w == b.w;
}

bool operator==(const Position &a, const Position &b)
{
  return a.tuple.get<0>() == b.tuple.get<0>() &&
         a.tuple.get<1>() == b.tuple.get<1>() &&
         a.tuple.get<2>() == b.tuple.get<2>();
}

bool operator<(const Position &a, const Position &b)
{
   if (a.tuple.get<0>() < b.tuple.get<0>())
     return true;
   if (b.tuple.get<0>() < a.tuple.get<0>())
     return false;
   // a1==b1: continue with element 2
   if (a.tuple.get<1>() < b.tuple.get<1>())
     return true;
   if (b.tuple.get<1>() < a.tuple.get<1>())
     return false;
   // a2 == b2: continue with element 3
   if (a.tuple.get<2>() < b.tuple.get<2>())
     return true;
   return false; // early out
}

std::size_t hash_value(const Position &e)
{
  std::size_t seed = 0;
  boost::hash_combine(seed, e.tuple.get<0>());
  boost::hash_combine(seed, e.tuple.get<1>());
  boost::hash_combine(seed, e.tuple.get<2>());
  return seed;
}
  
smallVolume::smallVolume(int sz)
{
   size = sz;
}

void smallVolume::fill()
{
   int x,y,z;
   for(x = 0; x < size; x++)
   {
      for (y = 0; y < size; y++)
      {
         for (z = 0; z < size; z++)
         {
            set(x,y,z,1);
         }
      }
   }
}

bool smallVolume::blockLeftVisible(int x, int y, int z)
{
   return not(is_solid(x-1,y,z));
}

bool smallVolume::blockRightVisible(int x, int y, int z)
{
   return not(is_solid(x+1,y,z));
}

bool smallVolume::blockAboveVisible(int x, int y, int z)
{
   return not(is_solid(x,y+1,z));
}

bool smallVolume::blockBelowVisible(int x, int y, int z)
{
   return not(is_solid(x,y-1,z));
}

bool smallVolume::blockFrontVisible(int x, int y, int z)
{
   return not(is_solid(x,y,z+1));
}

bool smallVolume::blockBackVisible(int x, int y, int z)
{
   return not(is_solid(x,y,z-1));
}

void smallVolume::empty()
{
   volumeData.clear();
}

bool smallVolume::is_solid(int x, int y, int z) 
{
   Position key(x,y,z);
   return not (volumeData.find(key) == volumeData.end());
}

byte smallVolume::get(int x, int y, int z)
{
   Position key(x,y,z);
   return volumeData[key].blockType;
}

void smallVolume::set(int x, int y, int z, byte value)
{
   Position key(x,y,z);
   block element;
   if(value != 0)
   {
      element.blockType = value;
      volumeData[key] = element;
   }
   else if(is_solid(x,y,z))
   {
      volumeData.erase(key);
   }
}