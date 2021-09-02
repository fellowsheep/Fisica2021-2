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

	mouseJoint = NULL;
}

void PhysicsManager::update()
{
	world->Step(timeStep, velocityIterations, positionIterations);
	//world->ClearForces();
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

//Para a mouseJoint
class QueryCallback : public b2QueryCallback
{
public:
	QueryCallback(const b2Vec2& point)
	{
		m_point = point;
		m_fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture) override
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;

				// We are done, terminate the query.
				return false;
			}
		}

		// Continue the query.
		return true;
	}

	b2Vec2 m_point;
	b2Fixture* m_fixture;
};

void PhysicsManager::MouseDown(const b2Vec2& p)
{
	mouseWorld = p;

	//std::cout << p.x << " " << p.y << std::endl;
	

	if (mouseJoint != NULL)
	{
		return;
	}

	// Make a small box.
	b2AABB aabb;
	b2Vec2 d;
	d.Set(0.001f, 0.001f);
	aabb.lowerBound = p - d;
	aabb.upperBound = p + d;

	// Query the world for overlapping shapes.
	QueryCallback callback(p);
	world->QueryAABB(&callback, aabb);

	if (callback.m_fixture)
	{
		float frequencyHz = 5.0f;
		float dampingRatio = 0.7f;

		b2Body* body = callback.m_fixture->GetBody();
		b2MouseJointDef jd;
		jd.bodyA = groundBody;
		jd.bodyB = body;
		jd.target = p;
		jd.maxForce = 1000.0f * body->GetMass();
		b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);

		mouseJoint = (b2MouseJoint*)world->CreateJoint(&jd);
		body->SetAwake(true);
	}
}

void PhysicsManager::MouseUp(const b2Vec2& p)
{
	if (mouseJoint)
	{
		world->DestroyJoint(mouseJoint);
		mouseJoint = NULL;
	}

}

void PhysicsManager::MouseMove(const b2Vec2& p)
{
	mouseWorld = p;

	if (mouseJoint)
	{
		mouseJoint->SetTarget(p);
	}
}
