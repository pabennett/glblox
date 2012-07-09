#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>

typedef unsigned char byte;

typedef struct{
   float x, y, z, w;
} vector4f;

typedef struct{
   float x, y, z;
} vector3f;

struct vector3i {
   int x, y, z;
   vector3i(int x =0,int y=0,int z=0) : x(x), y(y), z(z) {}
};

bool operator<(const vector3i &a, const vector3i &b)
{
   if (a.x < b.x)
     return true;
   if (b.x < a.x)
     return false;
   // a1==b1: continue with element 2
   if (a.y < b.y)
     return true;
   if (b.y < a.y)
     return false;
   // a2 == b2: continue with element 3
   if (a.z < b.z)
     return true;
   return false; // early out
}

std::size_t hash_value(const vector3i &e)
{
  std::size_t seed = 0;
  boost::hash_combine(seed, e.x);
  boost::hash_combine(seed, e.y);
  boost::hash_combine(seed, e.z);
  return seed;
}

typedef struct{
   int x, y;
} vector2i;

struct Position {
  Position(int x, int y, int z)
    : tuple(x,y,z) {}
  boost::tuples::tuple<int, int, int> tuple;
};

