#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Para armazenar o ponteiro do b2Body 
#include "PhysicsManager.h"


class RigidBody
{
public:
	RigidBody();
	~RigidBody();
	inline void setBody(b2Body *_body) { body = _body; }
	inline b2Body *getBody() { return body; }
protected:
	b2Body *body;
};
