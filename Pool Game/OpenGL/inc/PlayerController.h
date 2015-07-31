//-------------------------------------
//Author: Tommas Solarino
//description: This class handles the player cotroller
//-------------------------------------

#ifndef PLAYER_CONTROLLER
#define PLAYER_CONTROLLER

#include "MyControllerHitReport.h"
#include "AddWidget.h"
#include "glm/ext.hpp"

#include <PxPhysicsAPI.h>
#include <PxScene.h>
using namespace physx;


#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

class PlayerController
{
public:
	PlayerController(PxMaterial* _material, PxScene* _scene, glm::vec3 _startPos, GLFWwindow *_pWindow);
	~PlayerController();

	void Update(float _dt);
	void Draw();

private:
	GLFWwindow *m_pWindow;

	MyControllerHitReport *m_myHitReport;
	PxControllerManager* gCharacterManager;
	PxController* gPlayerController;

	float m_characterYVelocity;
	float m_characterRotation;
	float m_playerGravity;

	PxExtendedVec3 m_startPosition;
	AddWidget *m_addWidget;
};
#endif