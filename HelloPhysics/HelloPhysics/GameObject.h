#pragma once
#include "Shader.h"
#include "RigidBody.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void setShader(Shader *shader);
	void setVAO(GLuint VAO);
	void setTransform(glm::mat4 transform);

	void setRotation(float angle, glm::vec3 axis, bool reset = true);
	void setTranslation(glm::vec3 displacements, bool reset = true);
	void setScale(glm::vec3 scaleFactors, bool reset = true);

	void draw();

	void update();

	void setPhysics(bool _isRigidBody);

	inline glm::vec2 getDimensions() { return dimensions; }
	inline void setBody(b2Body *_body) { body = new RigidBody;  body->setBody(_body); }
	inline b2Body *getBody() { return body->getBody(); }

	void setAsBox();

	GLuint getVAO() { return VAO;  } //provisorio

protected:
	Shader *shader; //nesse caso, ponteiro para o shader do scene manager

	//scene attributes
	GLuint VAO;

	//Transformations - Model Matrix
	glm::mat4 model;

	//Ponto a revisar: manter fatores de escala, rot, trans..
	glm::vec2 dimensions; //largura e altura (sprite)
	glm::vec3 position;
	float angle;

	//Se é físico
	bool isRigidBody;
	RigidBody *body;
};

