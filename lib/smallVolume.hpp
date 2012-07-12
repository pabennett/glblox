#include <vector>

typedef struct{
   byte blockType;      // The material type of the block.
} block;

typedef boost::unordered_map<Position, block> smallVolumeData;
typedef boost::unordered_map<Position, block>::iterator iterator;

// Wrapper for STL map class
class smallVolume
{ 
   public:
      smallVolume(int);
      // Capacity
      bool is_empty();
      bool is_modified();
      bool is_full();
      bool is_solid(int, int, int);
      bool is_compressed();
      // Lookup
      byte get(int, int, int);
      bool blockLeftVisible(int, int, int);        // Neighbour status checks.
      bool blockRightVisible(int, int, int);       // Neighbour status checks.
      bool blockAboveVisible(int, int, int);       // Neighbour status checks.
      bool blockBelowVisible(int, int, int);       // Neighbour status checks.
      bool blockFrontVisible(int, int, int);       // Neighbour status checks.
      bool blockBackVisible(int, int, int);        // Neighbour status checks.
      bool leftBorderFull();                       // Self border status.
      bool rightBorderFull();                      // Chunk border status.
      bool topBorderFull();                        // Chunk border status.
      bool bottomBorderFull();                     // Chunk border status.
      bool frontBorderFull();                      // Chunk border status.
      bool backBorderFull();                       // Chunk border status.
      // Modification
      void yRangeSet(int, int, int, int, byte);
      void empty();
      void set(int, int, int, byte);
      void fill();
      void clearModifiedState();
      void uncompress();
      // Iterators
      iterator begin() { return volumeData.begin(); }
      iterator end() { return volumeData.end(); }
   protected:
      int size;
      bool modified;
      bool compressedFull;    // The volume is full and compressed.
      bool compressedArb;     // The volume contains arbitrary data and is compressed.
      smallVolumeData volumeData;
};