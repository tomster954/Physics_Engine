#include "PlayerController.h"
#include "MyControllerHitReport.h"

#include "glm/ext.hpp"
#include <PxPhysicsAPI.h>
using namespace physx;

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

PlayerController::PlayerController(PxMaterial* _material, PxScene* _scene, glm::vec3 _startPos, GLFWwindow *_pWindow)
{
	m_pWindow = _pWindow;

	m_addWidget = new AddWidget();

	m_startPosition.x = _startPos.x;
	m_startPosition.y = _startPos.y;
	m_startPosition.z = _startPos.z;

	m_myHitReport = new MyControllerHitReport();
	gCharacterManager = PxCreateControllerManager(*_scene);
	
	//describe our controller...
	PxCapsuleControllerDesc desc;
	desc.height = 1.6f;
	desc.radius = 0.4f;
	desc.position.set(0, 0, 0);
	desc.material = _material;
	desc.reportCallback = m_myHitReport; //connect it to our collision detection routine
	desc.density = 10;
	//create the layer controller
	gPlayerController = gCharacterManager->createController(desc);
	gPlayerController->setPosition(m_startPosition);
	//set up some variables to control our player with
	m_characterYVelocity = 0; //initialize character velocity
	m_characterRotation = 0; //and rotation
	m_playerGravity = -0.5f; //set up the player gravity
	m_myHitReport->clearPlayerContactNormal(); //initialize the contact normal (what we are in contact with)
}

PlayerController::~PlayerController()
{

}
void PlayerController::Update(float _dt)
{
	bool onGround; //set to true if we are on the ground
	float movementSpeed = 10.0f; //forward and back movement speed
	float rotationSpeed = 1.0f; //turn speed
	//check if we have a contact normal. if y is greater than .3 we assume this is solid ground.This is a rather primitive way to do this.Can you do better ?
	if (m_myHitReport->getPlayerContactNormal().y > 0.3f)
	{
		m_characterYVelocity = -0.1f;
		onGround = true;
	}
	else
	{
		m_characterYVelocity += m_playerGravity * _dt;
		onGround = false;
	}
	m_myHitReport->clearPlayerContactNormal();
	const PxVec3 up(0, 1, 0);
	
	//scan the keys and set up our intended velocity based on a global transform
	PxVec3 velocity(0, m_characterYVelocity, 0);

	if (glfwGetKey(m_pWindow, GLFW_KEY_UP) == GLFW_PRESS)
		velocity.z += movementSpeed * _dt;
	if (glfwGetKey(m_pWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
		velocity.z -= movementSpeed * _dt;
	if (glfwGetKey(m_pWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
		velocity.x += movementSpeed * _dt;
	if (glfwGetKey(m_pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
		velocity.x -= movementSpeed * _dt;
	if (glfwGetKey(m_pWindow, GLFW_KEY_SPACE) == GLFW_PRESS && onGround)
		velocity.y += movementSpeed * 10 *_dt;

	//To do.. add code to control z movement and jumping
	float minDistance = 0.001f;
	PxControllerFilters filter;
	//make controls relative to player facing
	PxQuat rotation(m_characterRotation, PxVec3(0, 1, 0));
	//move the controller
	gPlayerController->move(rotation.rotate(velocity), minDistance, _dt,
		filter);
}

void PlayerController::Draw()
{
	PxU32 nShapes = gPlayerController->getActor()->getNbShapes();
	PxShape** shapes = new PxShape*[nShapes];
	gPlayerController->getActor()->getShapes(shapes, nShapes);
			// Render all the shapes in the physx actor (for early tutorials there is just one)
	while (nShapes--) 
	{
		m_addWidget->AddWidgetFunc(shapes[nShapes], gPlayerController->getActor());
	}
	delete[] shapes;
	
}
