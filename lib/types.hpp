#include <boost/tuple/tuple.hpp>

typedef unsigned char byte;

typedef struct{
   float x, y, z, w;
} vector4f;

typedef struct{
   float x, y, z;
} vector3f;

typedef struct{
   int x, y, z;
} vector3i;

typedef struct{
   int x, y;
} vector2i;

struct Position {
  Position(int x, int y, int z)
    : tuple(x,y,z) {}
  boost::tuples::tuple<int, int, int> tuple;
};

