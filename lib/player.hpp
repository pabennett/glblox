class Player
{                      
   public:      
      // Construct
      Player(float, float, float, Camera*);
      // Destruct
      ~Player();
      void move(float, float, float);
      void jump();
      void orient(float, float);
      void setCameraMVP();
      float getPositionX();
      float getPositionY();
      float getPositionZ();
      glm::vec3 getPosition();
      glm::mat4 getPlayerCamMVP();
   private:
      Camera* playerCamera;
      glm::vec3 position;
      glm::fquat orientation;
};
      
      