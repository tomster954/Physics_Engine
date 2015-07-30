#include "PhysXState.h"
#include <Gizmos.h>
#include "Ragdoll.h"

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
	SetUpPhysX();
}

PhysXState::~PhysXState()
{
	g_PhysicsScene->release();
	g_Physics->release();
	g_PhysicsFoundation->release();
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
	Reset();

	m_ragdoll = new Ragdoll(g_Physics, g_PhysicsMaterial, g_PhysicsScene);
}

void PhysXState::Update(float _dt)
{
	Gizmos::clear();

	if (glfwGetKey(m_pWindow, GLFW_KEY_R) == GLFW_PRESS)
		Reset();

	if (_dt <= 0)
		return;

	g_PhysicsScene->simulate(_dt);
	
	while (g_PhysicsScene->fetchResults() == false)
	{
		// don’t need to do anything here yet but we have to fetch results
	}

	// Add widgets to represent all the phsyX actors which are in the scene
	for (auto actor : g_PhysXActors)
	{
		PxU32 nShapes = actor->getNbShapes();
		PxShape** shapes = new PxShape*[nShapes];
		actor->getShapes(shapes, nShapes);
		
		// Render all the shapes in the physx actor (for early tutorials there is just one)
		while (nShapes--)
			AddWidget(shapes[nShapes], actor);

		delete[] shapes;
	}
}

void PhysXState::Draw(Camera *_camera)
{
	glm::vec4 colour;
	colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
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

void PhysXState::AddWidget(PxShape* shape, PxRigidActor* actor)
{
	PxGeometryType::Enum type = shape->getGeometryType();
	switch (type)
	{
	case PxGeometryType::eBOX:
		AddBox(shape, actor);
		break;
	case PxGeometryType::ePLANE:
		AddPlane(shape, actor);
		break;
	}
}

void PhysXState::AddBox(PxShape* pShape, PxRigidActor* actor)
{
	//get the geometry for this PhysX collision volume
	PxBoxGeometry geometry;
	float width = 1, height = 1, length = 1;
	bool status = pShape->getBoxGeometry(geometry);
	if (status)
	{
		width = geometry.halfExtents.x;
		height = geometry.halfExtents.y;
		length = geometry.halfExtents.z;
	}
	//get the transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*pShape, *actor));
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
				m.column1.x, m.column1.y, m.column1.z, m.column1.w,
				m.column2.x, m.column2.y, m.column2.z, m.column2.w,
				m.column3.x, m.column3.y, m.column3.z, m.column3.w);
	
	glm::vec3 position;
	//get the position out of the transform
	position.x = m.getPosition().x;
	position.y = m.getPosition().y;
	position.z = m.getPosition().z;
	
	glm::vec3 extents = glm::vec3(width, height, length);
	glm::vec4 colour = glm::vec4(1, 0, 0, 1);
	
	if (actor->getName() != NULL && strcmp(actor->getName(), "Pickup1")) //seriously horrid hack so I can show pickups a different colour
		colour = glm::vec4(0, 1, 0, 1);
	
	//create our box gizmo
	Gizmos::addAABBFilled(position, extents, colour, &M);
}

void PhysXState::AddPlane(PxShape* pShape, PxRigidActor* actor)
{
	//get the geometry for this PhysX collision volume
	PxBoxGeometry geometry;
	float width = 1000, height = 0, length = 1000;

	//get the transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*pShape, *actor));
	
	glm::vec3 position;
	//get the position out of the transform
	position.x = m.getPosition().x;
	position.y = m.getPosition().y;
	position.z = m.getPosition().z;

	glm::vec3 extents = glm::vec3(width, height, length);
	glm::vec4 colour = glm::vec4(1, 0.8f, 0.5f, 1);

	//create our box gizmo
	Gizmos::addAABBFilled(position, extents, colour);
}

void PhysXState::CreateBoxes()
{
	//Box vars
	float density = 100;
	PxBoxGeometry box(0.5f, 0.5f, 0.5f);
	PxTransform transform(PxVec3(0, 1, 0));
	PxRigidDynamic* dynamicActor;

	float boxes = 10;
	float rnd;

	//falling boxes
	for (int i = 0; i < boxes; i++)
	{
		rnd = rand() % 50;

		transform.p.y = transform.p.y + 1;
		transform.p.x = (rnd - 25.0f) / 10.0f;
		transform.p.z = (rnd - 25.0f) / 10.0f;

		dynamicActor = PxCreateDynamic(*g_Physics, transform, box, *g_PhysicsMaterial, density);
		
		//add it to the physX scene
		g_PhysicsScene->addActor(*dynamicActor);
		g_PhysXActors.push_back(dynamicActor);
	}

	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));

	float yExtent = 5;
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
}