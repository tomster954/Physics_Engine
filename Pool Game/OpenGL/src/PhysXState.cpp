#include "PhysXState.h"
#include <Gizmos.h>
#include "Ragdoll.h"
#include "ParticleEmitter.h"

#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd/PxVisualDebugger.h>
#include "glm/ext.hpp"

#include <stdlib.h>

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

using namespace physx;

class myAllocator : public PxAllocatorCallback
{
public:
	virtual ~myAllocator() {}
	virtual void* allocate(size_t size, const char* typeName, const char* filename, int line)
	{
		void* pointer = _aligned_malloc(size, 16);
		return pointer;
	}
	virtual void deallocate(void* ptr)
	{
		_aligned_free(ptr);
	}
};

PhysXState::PhysXState(GLFWwindow *_pWindow) : 
m_project(	1, 0, 0, 1,
			0, 1, 0, 1,
			0, 0, 1, 1,
			0, 0, 0, 1)
{
	m_pWindow = _pWindow;

	m_addWidget = new AddWidget();

	SetUpPhysX();
	InitParticles();
}

PhysXState::~PhysXState()
{
	g_PhysicsScene->release();
	g_Physics->release();
	g_PhysicsFoundation->release();
	delete m_ragdoll;
	delete m_playerController;
}

void PhysXState::Reset()
{
	for each(auto i in g_PhysXActors)
		g_PhysicsScene->removeActor(*i);

	g_PhysXActors.clear();
	
	//add a plane
	PxTransform pose = PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxHalfPi*1.0f, PxVec3(0.0f, 0.0f, 1.0f)));
	plane = PxCreateStatic(*g_Physics, pose, PxPlaneGeometry(), *g_PhysicsMaterial);

	//add it to the physX scene
	g_PhysicsScene->addActor(*plane);
	g_PhysXActors.push_back(plane);

	CreateBoxes();
}

void PhysXState::SetUpPhysX()
{
	PxAllocatorCallback *myCallback = new myAllocator();
	g_PhysicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *myCallback, gDefaultErrorCallback);
	g_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *g_PhysicsFoundation, PxTolerancesScale());
	PxInitExtensions(*g_Physics);

	//create physics material 
	g_PhysicsMaterial = g_Physics->createMaterial(0.2f, 0.2f, 0.2f);
	PxSceneDesc sceneDesc(g_Physics->getTolerancesScale());
	
	sceneDesc.gravity = PxVec3(0, -10.0f, 0);
	sceneDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	
	g_PhysicsScene = g_Physics->createScene(sceneDesc);
	SetUpVisualDebugger();
	
	m_ragdoll = new Ragdoll(g_Physics, g_PhysicsMaterial, g_PhysicsScene);
	m_playerController = new PlayerController(g_PhysicsMaterial, g_PhysicsScene, glm::vec3(0, 10, -10), m_pWindow);
	Reset();
}

void PhysXState::Update(float _dt)
{
	Gizmos::clear();

	if (glfwGetKey(m_pWindow, GLFW_KEY_R) == GLFW_PRESS)
		Reset();

	g_PhysicsScene->simulate(_dt);


	while (g_PhysicsScene->fetchResults() == false)
	{
		//dont need to do anythig here yet but have to fetch results
	}

	if (m_particleEmitter)
	{
		m_particleEmitter->update(_dt);
		//render all our particles
		m_particleEmitter->renderParticles();
	}

	m_playerController->Update(_dt);
}

void PhysXState::Draw(Camera *_camera)
{
	glm::vec4 colour;
	colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	m_ragdoll->Draw();
	m_playerController->Draw();

	// Add widgets to represent all the phsyX actors which are in the scene
	for (auto actor : g_PhysXActors)
	{
		PxU32 nShapes = actor->getNbShapes();
		PxShape** shapes = new PxShape*[nShapes];
		actor->getShapes(shapes, nShapes);

		// Render all the shapes in the physx actor (for early tutorials there is just one)
		while (nShapes--)
			m_addWidget->AddWidgetFunc(shapes[nShapes], actor);

		delete[] shapes;
	}

	Gizmos::draw(_camera->getProjectionView());
}

void PhysXState::SetUpVisualDebugger()
{
	// check if PvdConnection manager is available on this platform
	if (g_Physics->getPvdConnectionManager() == NULL)
		return;
	
	// setup connection parameters
	const char* pvd_host_ip = "127.0.0.1";
	// IP of the PC which is running PVD
	int port = 5425;
	// TCP port to connect to, where PVD is listening
	
	unsigned int timeout = 100;
	// timeout in milliseconds to wait for PVD to respond,
	//consoles and remote PCs need a higher timeout.
	PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();
	
	// and now try to connectPxVisualDebuggerExt
	auto theConnection = PxVisualDebuggerExt::createConnection(g_Physics->getPvdConnectionManager(), pvd_host_ip, port, timeout, connectionFlags);
}

void PhysXState::CreateBoxes()
{
	//Box vars
	float density = 1;
	PxBoxGeometry box(1.0f , 1.0f, 1.0f);
	PxTransform transform(PxVec3(0, 1, 0));
	PxRigidDynamic* dynamicActor;

	float boxes = 10;
	float rndX, rndZ;
	//falling boxes
	for (int i = 0; i < boxes; i++)
	{
		rndX = (float)(rand() % 50);
		rndZ = (float)(rand() % 50);

		transform.p.y = transform.p.y + 1;
		transform.p.x = (rndX - 25.0f) / 10.0f;
		transform.p.z = (rndZ - 25.0f) / 10.0f;

		dynamicActor = PxCreateDynamic(*g_Physics, transform, box, *g_PhysicsMaterial, density);
		
		//add it to the physX scene
		g_PhysicsScene->addActor(*dynamicActor);
		g_PhysXActors.push_back(dynamicActor);
	}

	//Walls
	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	float yExtent = 1;
	float longSide = 10;
	float thinSide = 1;
	PxBoxGeometry side1(longSide, yExtent, thinSide);
	PxBoxGeometry side2(thinSide, yExtent, longSide);

	pose = PxTransform(PxVec3(0.0f, yExtent, longSide));
	PxRigidStatic* boxWall = PxCreateStatic(*g_Physics, pose, side1, *g_PhysicsMaterial);

	g_PhysicsScene->addActor(*boxWall);
	g_PhysXActors.push_back(boxWall);

	pose = PxTransform(PxVec3(0.0f, yExtent, -longSide));
	boxWall = PxCreateStatic(*g_Physics, pose, side1, *g_PhysicsMaterial);
	g_PhysicsScene->addActor(*boxWall);
	g_PhysXActors.push_back(boxWall);
	
	pose = PxTransform(PxVec3(longSide, yExtent, 0));
	boxWall = PxCreateStatic(*g_Physics, pose, side2, *g_PhysicsMaterial);
	g_PhysicsScene->addActor(*boxWall);
	g_PhysXActors.push_back(boxWall);
	
	pose = PxTransform(PxVec3(-longSide, yExtent, 0));
	boxWall = PxCreateStatic(*g_Physics, pose, side2, *g_PhysicsMaterial);
	g_PhysicsScene->addActor(*boxWall);
	g_PhysXActors.push_back(boxWall);
	//------------------------------------
}

void PhysXState::InitParticles()
{
	//create our particle system
	PxParticleFluid* pf;
	// create particle system in PhysX SDK
	// set immutable properties.
	PxU32 maxParticles = 500;
	bool perParticleRestOffset = false;

	pf = g_Physics->createParticleFluid(maxParticles, perParticleRestOffset);
	pf->setRestParticleDistance(1.5f);
	pf->setDynamicFriction(0.1);
	pf->setStaticFriction(0.1);
	pf->setDamping(0.1);
	pf->setParticleMass(.1);
	pf->setRestitution(0);

	pf->setParticleBaseFlag(PxParticleBaseFlag::eCOLLISION_TWOWAY, true);
	pf->setStiffness(100);
	if (pf)
	{
		g_PhysicsScene->addActor(*pf);
		m_particleEmitter = new ParticleFluidEmitter(maxParticles, PxVec3(0, 10, 0), pf, 0.05f);
		m_particleEmitter->setStartVelocityRange(-10.0f, 0, -10.0f, 10.0f, 0, 10.0f);
		m_particleEmitter->setSize(glm::vec3(0.5f, 0.5f, 0.5f));
		m_particleEmitter->setColour(glm::vec4(0, 0, 0.9f, 1));
	}
}