class Player
{                      
   public:      
      // Construct
      Player(Camera*, World*, GLuint);
      // Destruct
      ~Player();
      void move(float, float, float);
      void jump();
      void orient(float, float);
      void setCameraMVP();
      void update(float, bool, bool, bool, bool);
      float getPositionX();
      float getPositionY();
      float getPositionZ();
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
      bool collisionStatusLeft;
      bool collisionStatusRight;
      bool collisionStatusFront;
      bool collisionStatusBack;
      bool collisionStatusTop;
      bool collisionStatusBottom;
      bool getCollisionStatusLeft();
      bool getCollisionStatusRight();
      bool getCollisionStatusFront();
      bool getCollisionStatusBack();
      bool getCollisionStatusTop();
      bool getCollisionStatusBottom();
   private:
      Camera* playerCamera;
      World* world;
      
      glm::vec3 position;
      glm::fquat orientation;
      glm::vec3 velocity;
      AABB bbox;
      float bboxSize;

      std::vector<vertex> playerVertices;    // Array of vertices: front faces.
      GLuint playerVerticesVBO;
      GLuint posAttrib;
      GLuint worldPosAttrib;
      GLuint program;
      bool onGround;
      bool AABBCollisionTest(std::vector<AABB>, AABB);
};
      
      