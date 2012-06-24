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
      bool is_solid(int, int, int);
      byte get(int, int, int);
      void set(int, int, int, byte);
      void fill();
      void empty();
      bool is_empty();
      bool is_modified();
      void clearModifiedState();
      bool blockLeftVisible(int, int, int);
      bool blockRightVisible(int, int, int);
      bool blockAboveVisible(int, int, int);
      bool blockBelowVisible(int, int, int);
      bool blockFrontVisible(int, int, int);
      bool blockBackVisible(int, int, int);
      // Expose volume data with iterators only
      iterator begin() { return volumeData.begin(); }
      iterator end() { return volumeData.end(); }
   protected:
      int size;
      bool modified;
      smallVolumeData volumeData;
};