#include "AABB.cpp"

class Player
{                      
   public:      
      // Construct
      Player(Camera*, World*, GLuint);
      // Destruct
      ~Player();
      void move(float, float, float);
      void setFlightMode(bool);
      void jump();
      void orient(float, float);
      void setCameraMVP();
      void update(float, bool, bool, bool, bool);
      float getPositionX();
      float getPositionY();
      float getPositionZ();
      float voxelSize;
      glm::vec3 getPosition();
      glm::mat4 getPlayerCamMVP();
      // Player Camera Stuff
      glm::mat4 MVP;
      glm::mat4 viewMatrix;
      glm::mat4 projectionMatrix;
      glm::mat4 modelMatrix;
      float getPlayerVelocityX();
      float getPlayerVelocityY();
      float getPlayerVelocityZ();
      void draw();
   private:
      Camera* playerCamera;
      World* world;
      GLuint program;
      
      glm::vec3 position;
      glm::fquat orientation;
      glm::vec3 velocity;
      
      // Properties
      bool gravityEnabled;
      bool collisionTestsEnabled;
      bool flightEnabled;
      
      // Collision Detection
      AABB playerBox;
      bool onGround;
      bool leftCollisionTest();
      bool rightCollisionTest();
      bool frontCollisionTest();
      bool backCollisionTest();
      bool topCollisionTest();
      bool bottomCollisionTest();
      bool touchingVoxelLeftAt(int, int, int);
      bool touchingVoxelRightAt(int, int, int);
      bool touchingVoxelFrontAt(int, int, int);
      bool touchingVoxelBackAt(int, int, int);
      bool touchingVoxelAboveAt(int, int, int);
      bool touchingVoxelBelowAt(int, int, int);
};
      
      