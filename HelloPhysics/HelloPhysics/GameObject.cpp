#include "GameObject.h"



GameObject::GameObject()
{
	model = glm::mat4();
}


GameObject::~GameObject()
{
}

void GameObject::setShader(Shader * shader)
{
	this->shader = shader;
	shader->Use();
}

void GameObject::setVAO(GLuint VAO)
{
	this->VAO = VAO;
}

void GameObject::setTransform(glm::mat4 transform)
{
	this->model = transform;
	// Get their uniform location
	GLint modelLoc = glGetUniformLocation(shader->Program, "model");
	// Pass them to the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void GameObject::setRotation(float angle, glm::vec3 axis, bool reset)
{
	if (reset) model = glm::mat4();
	model = glm::rotate(model, angle, axis);
}

void GameObject::setTranslation(glm::vec3 displacements, bool reset)
{
	if (reset) model = glm::mat4();
	model = glm::translate(model, displacements);
}

void GameObject::setScale(glm::vec3 scaleFactors, bool reset)
{
	if (reset) model = glm::mat4();
	model = glm::scale(model, scaleFactors);
	dimensions = scaleFactors;
}

void GameObject::draw()
{
	//considerando que possui EBO
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GameObject::update()
{
	// E então enviamos para o shader como uma variável mat4 uniforme
	// Get their uniform location
	if (isRigidBody)
	{
		//Bem rudimentar ainda, sem buscar fixtures nem nada, só pra sprites mesmo
		b2Vec2 pos = body->getBody()->GetPosition();
		float ori = body->getBody()->GetAngle();

		position = glm::vec3(pos.x, pos.y, 0.0);
		angle = ori;

		setTranslation(position);
		setRotation(angle, glm::vec3(0.0f, 0.0f, 1.0f), false);
		setScale(glm::vec3(dimensions.x, dimensions.y, 1.0), false); //pra acumular as transforms, não pode resetar!
	}

	GLint modelLoc = glGetUniformLocation(shader->Program, "model");
	// Pass them to the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void GameObject::setPhysics(bool _isRigidBody)
{
	isRigidBody = _isRigidBody;
	
}

void GameObject::setAsBox()
{
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Colors           
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f,    // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,    // Bottom Right
		-0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   // Bottom Left
		-0.5f,  0.5f, 0.0f,   0.0f, 0.7f, 0.5f,   // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	//Criação do Buffer com a geometria (VAO) --> TODO: isso precisaria ser diferenciado para polígonos que não sejam sprites
	GLuint VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

}

