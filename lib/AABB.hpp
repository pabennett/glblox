class AABB
{                      
   public:      
      // Default Construct
      AABB();
      // Construct
      AABB(glm::vec3, glm::vec3); 
      // Access
      glm::vec3 minVector();
      glm::vec3 maxVector();
      // Modifiers
      void setPosition(glm::vec3);
      // Collision tests
      collisionStatus collisionTest(AABB);
      bool leftCollisionTest(AABB);
      bool rightCollisionTest(AABB);  
      bool backCollisionTest(AABB);
      bool frontCollisionTest(AABB); 
      bool bottomCollisionTest(AABB);
      bool topCollisionTest(AABB);  
      bool contains(AABB);
      // Variables
      glm::vec3 extents;
      glm::vec3 centre;
      glm::vec3 max;    // Computed from extents and centre
      glm::vec3 min;    // Computed from extents and centre
};