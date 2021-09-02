#pragma once
#include <Box2D/Box2D.h>
#include <vector>
#include <iostream> //para alguns debugs

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 color;
	glm::vec3 normal;
};

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();
	void initialize(b2Vec2 gravity);
	inline b2World* getWorld() { return world; }
	void update();
	void finish();

	//Continuar em casa
	b2Body *createBox(glm::vec2 pos, glm::vec2 dim, float density, float friction, float restitution, bool isDynamic=false);
	
	inline void setGround(b2Body* b) { groundBody = b; }

	void MouseDown(const b2Vec2& p);
	void MouseUp(const b2Vec2& p);
	void MouseMove(const b2Vec2& p);


protected:
	
	b2World *world;
	float timeStep;
	int velocityIterations;
	int positionIterations;

	b2MouseJoint* mouseJoint;
	b2Vec2 mouseWorld;
	b2Body* groundBody;
};

