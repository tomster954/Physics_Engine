//-------------------------------------
//Author: Tommas Solarino
//Description: this is the state that uses PhysX
//-------------------------------------

#ifndef PHYSXSTATE
#define PHYSXSTATE

#include "Camera.h"
#include "Ragdoll.h"
#include "ParticleFluidEmitter.h"
#include "AddWidget.h"
#include "PlayerController.h"
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
	void InitParticles();

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

	Ragdoll *m_ragdoll;

	ParticleFluidEmitter *m_particleEmitter;

	AddWidget *m_addWidget;
	PlayerController *m_playerController;
};

#endif