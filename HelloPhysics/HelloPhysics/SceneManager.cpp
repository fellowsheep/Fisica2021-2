#include "SceneManager.h"

//static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint width, height;


SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;
	
	//Inicializando o gerenciador da Física do mundo
	physics = new PhysicsManager;
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.8f);
	physics->initialize(gravity);


	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();

	

}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "Hello Transform", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);
	
	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Build and compile our shader program
	addShader("../shaders/transformations.vs", "../shaders/transformations.frag");

	//setup the scene -- LEMBRANDO QUE A DESCRIÇÃO DE UMA CENA PODE VIR DE ARQUIVO(S) DE 
	// CONFIGURAÇÃO
	setupScene();

	resized = true; //para entrar no setup da câmera na 1a vez

}

void SceneManager::addShader(string vFilename, string fFilename)
{
	shader = new Shader (vFilename.c_str(), fFilename.c_str());
}


void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void SceneManager::resize(GLFWwindow * window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}


void SceneManager::update()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (keys[GLFW_KEY_SPACE])
	{
		//------------------------------------
		//Criação do GameObject 1
		GameObject *obj = new GameObject;
		obj->setShader(shader);
		obj->setVAO(objs[0]->getVAO());

		//Geometria, dimensões (escala), posição (translação), orientação (rotação)
		//Escala: largura e altura (a sprite tem 1x1)
		obj->setScale(glm::vec3(1.5, 1.5, 1.0));

		//Se é corpo rígido, fazer a criação pelo Physics Manager
		//Definir: forma (box pra sprite), dimensoes (pegar do obj), 
		//atrito, restituição e densidade,
		//static ou dynamic
		b2Body *box = physics->createBox(glm::vec2(0.0, 0.0), obj->getDimensions(), 1.0, 0.5, 0.9, true);

		obj->setPhysics(true);
		obj->setBody(box);
		obj->setVAO(objs[0]->getVAO());

		objs.push_back(obj);

	}

	// testa as flags que indicaram eventos e modifica os atributos da cena (matrizes de transformanção)
	//..
	//..
	//if (keys[GLFW_KEY_UP]) { ... } 
	
	//// No caso aqui, apenas o primeiro objeto vai rotacionar e escalar
	//objs[0]->setTranslation(glm::vec3(-0.5, 0.0, 0.0));
	//objs[0]->setRotation((GLfloat)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f),false);
	//objs[0]->setScale(glm::vec3(0.25, 0.25, 1.0),false); //pra acumular as transforms, não pode resetar!

	////E o segundo vai sofrer uma translação e escalas diferentes em x e  y
	//objs[1]->setTranslation(glm::vec3(0.5, 0.0, 0.0));
	//objs[1]->setScale(glm::vec3(0.3, 1.5, 1.0), false);

	// Dica: se esses comportamentos forem padrão como os de cima (não mudam com o user), poderiam ser 
	// também estabelecidos dentro do gameobject. Neste caso, teríamos q armazenar no gameObject
	// atributos que armazenam os deslocamentos, fatores de escala e rotação inicial de cada objeto


	//Chama o passo da simulação Física
	physics->update();

	//E aqui testamos se houve mudança na câmera 2D (projeção ortográfica)
	if (resized) //se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}
}

void SceneManager::render()
{
	// Clear the colorbuffer
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < objs.size(); i++)
	{
		objs[i]->update(); //para atualizar os uniforms do objeto
		objs[i]->draw();
	}

	//for (int i = 0; i < nObjs; i++)
	//{
	//	objs[i]->update(); //para atualizar os uniforms do objeto
	//	objs[i]->draw(); //para fazer a drawcall propriamente
	//}
}

void SceneManager::run()
{
	// Render scene
	shader->Use();

	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Update method(s)
		update();

		//Render scene
		render();
		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


void SceneManager::setupScene()
{

//------------------------------------
	//Criação do GameObject 1
	GameObject *obj = new GameObject;
	obj->setShader(shader);
	obj->setAsBox();

	//Geometria, dimensões (escala), posição (translação), orientação (rotação)
	//Escala: largura e altura (a sprite tem 1x1)
	obj->setScale(glm::vec3(2.0, 3.0, 1.0));

	//Se é corpo rígido, fazer a criação pelo Physics Manager
	//Definir: forma (box pra sprite), dimensoes (pegar do obj), 
	//atrito, restituição e densidade,
	//static ou dynamic
	b2Body *box = physics->createBox(glm::vec2(0.0, 0.0), obj->getDimensions(), 1.0, 0.5, 0.5, true);

	obj->setPhysics(true);
	obj->setBody(box);
	obj->setAsBox();

	objs.push_back(obj);

	//------------------------------------------------
	//Criação do GameObject 2
	obj = new GameObject;
	obj->setShader(shader);
	obj->setAsBox();

	//Geometria, dimensões (escala), posição (translação), orientação (rotação)
	//Escala: largura e altura (a sprite tem 1x1)
	obj->setScale(glm::vec3(20.0, 0.5, 1.0));

	//Se é corpo rígido, fazer a criação pelo Physics Manager
	//Definir: forma (box pra sprite), dimensoes (pegar do obj), 
	//atrito, restituição e densidade,
	//static ou dynamic
	box = physics->createBox(glm::vec2(0.0, -9.0), obj->getDimensions(), 1.0, 0.5, 0.5, false);

	obj->setPhysics(true);
	obj->setBody(box);
	obj->setAsBox();

	objs.push_back(obj);
	//------------------------------------------------
	//Criação do GameObject 3
	obj = new GameObject;
	obj->setShader(shader);
	obj->setAsBox();

	//Geometria, dimensões (escala), posição (translação), orientação (rotação)
	//Escala: largura e altura (a sprite tem 1x1)
	obj->setScale(glm::vec3(0.5, 20.0, 1.0));

	//Se é corpo rígido, fazer a criação pelo Physics Manager
	//Definir: forma (box pra sprite), dimensoes (pegar do obj), 
	//atrito, restituição e densidade,
	//static ou dynamic
	box = physics->createBox(glm::vec2(-9.0, 0.0), obj->getDimensions(), 1.0, 0.5, 0.5, false);

	obj->setPhysics(true);
	obj->setBody(box);
	obj->setAsBox();

	objs.push_back(obj);
	//------------------------------------------------
	//Criação do GameObject 4
	obj = new GameObject;
	obj->setShader(shader);
	obj->setAsBox();

	//Geometria, dimensões (escala), posição (translação), orientação (rotação)
	//Escala: largura e altura (a sprite tem 1x1)
	obj->setScale(glm::vec3(0.5, 20.0, 1.0));

	//Se é corpo rígido, fazer a criação pelo Physics Manager
	//Definir: forma (box pra sprite), dimensoes (pegar do obj), 
	//atrito, restituição e densidade,
	//static ou dynamic
	box = physics->createBox(glm::vec2(9.0, 0.0), obj->getDimensions(), 1.0, 0.5, 0.5, false);

	obj->setPhysics(true);
	obj->setBody(box);
	obj->setAsBox();

	objs.push_back(obj);

	//Se tiver textura, inicializar o objeto de Sprite

	//nObjs = 2;
	//objs = new GameObject*[nObjs];

	////Setando (no braço) o primeiro objeto
	//objs[0] = new GameObject;
	//objs[0]->setShader(shader);
	//objs[0]->setVAO(VAO);
	//
	////Setando (no braço) o segundo objeto
	//objs[1] = new GameObject;
	//objs[1]->setShader(shader);
	//objs[1]->setVAO(VAO);
	

}

void SceneManager::setupCamera2D()
{
	//corrigindo o aspecto
	float ratio;
	float xMin = -10.0, xMax = 10.0, yMin = -10.0, yMax = 10.0, zNear = -1.0, zFar = 1.0;
	if (width >= height)
	{
		ratio = width / (float)height;
		//ratio = 1.0;
		projection = glm::ortho(xMin*ratio, xMax*ratio, yMin, yMax, zNear, zFar);
	}
	else
	{
		ratio = height / (float)width;
		projection = glm::ortho(xMin, xMax, yMin*ratio, yMax*ratio, zNear, zFar);
	}

	// Get their uniform location
	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
