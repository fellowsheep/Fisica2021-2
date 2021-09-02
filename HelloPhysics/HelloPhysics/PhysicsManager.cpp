#include "PhysicsManager.h"



PhysicsManager::PhysicsManager()
{
}


PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::initialize(b2Vec2 gravity)
{
	// Inicializa a biblioteca Box2D
	world = new b2World(gravity);

	// Define os parâmetro para a simulação
	// Quanto maior, mais preciso, porém, mais lento - VALORES RECOMENDADOS no manual
	velocityIterations = 6;
	positionIterations = 2;
	timeStep = 1.0f / 60.0f; //60 Hz -- 1/60 seconds - pensando em tempo real para jogos (60FPS)
}

void PhysicsManager::update()
{
	world->Step(timeStep, velocityIterations, positionIterations);
	world->ClearForces();
}

b2Body * PhysicsManager::createBox(glm::vec2 pos, glm::vec2 dim, float density, float friction, float restitution, bool isDynamic)
{
	b2Body *novoObjeto;

	//Primeiro, criamos a definição do corpo
	b2BodyDef bodyDef;
	bodyDef.position.Set(pos.x, pos.y);
	if (isDynamic) //por enquanto não faz cinemático
	{
		bodyDef.type = b2_dynamicBody;
	}
	else
	{
		bodyDef.type = b2_staticBody;
	}
	
	//Estamos usando uma forma de poligono, que pode ter até 8 vértices
	b2PolygonShape forma;
	forma.SetAsBox(dim.x/2.0, dim.y/2.0);

	//Depois, criamos uma fixture que vai conter a forma do corpo
	b2FixtureDef fix;
	fix.shape = &forma;
	//Setamos outras propriedades da fixture
	fix.density = density;
	fix.friction = friction;
	fix.restitution = restitution;

	//Por fim, criamos o corpo...
	novoObjeto = world->CreateBody(&bodyDef);
	//... e criamos a fixture do corpo 	
	novoObjeto->CreateFixture(&fix);

	return novoObjeto;
}
