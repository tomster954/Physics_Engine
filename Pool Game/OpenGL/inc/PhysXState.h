//-------------------------------------
//Author: Tommas Solarino
//Description: this is the state that uses PhysX
//-------------------------------------

#ifndef PHYSXSTATE
#define PHYSXSTATE

#include "Camera.h"
struct GLFWwindow;

#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd/PxVisualDebugger.h>
#include <vector>

using namespace physx;

class PhysXState
{
public:
	PhysXState(GLFWwindow *_pWindow);
	~PhysXState();

	void Update(float _dt);
	void Draw(Camera *_camera);
	
	void CreateBoxes();

	void Reset();

	void SetUpPhysX();
	void SetUpVisualDebugger();
	void SetupTutorial1();

	void AddWidget(PxShape* shape, PxRigidActor* actor);
	void AddBox(PxShape* pShape, PxRigidActor* actor);
	void AddPlane(PxShape* pShape, PxRigidActor* actor);

private:
	GLFWwindow *m_pWindow;
	glm::mat4 m_project;

	PxFoundation* g_PhysicsFoundation;
	PxPhysics* g_Physics;
	PxScene* g_PhysicsScene;
	PxDefaultErrorCallback gDefaultErrorCallback;
	PxDefaultAllocator gDefaultAllocatorCallback;
	PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;
	PxMaterial* g_PhysicsMaterial;
	PxMaterial* g_boxMaterial;
	PxCooking* g_PhysicsCooker;
	PxRigidStatic* plane;

	std::vector<PxRigidActor*> g_PhysXActors;
};

#endif