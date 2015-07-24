#include "PhysXState.h"
#include <Gizmos.h>

PhysXState::PhysXState(GLFWwindow *_pWindow) : 
m_project(	1, 0, 0, 1,
			0, 1, 0, 1,
			0, 0, 1, 1,
			0, 0, 0, 1)
{
	m_pWindow = _pWindow;
}

PhysXState::~PhysXState()
{

}

void PhysXState::Update(float _dt)
{

}

void PhysXState::Draw(Camera *_camera)
{
	Gizmos::clear();
	glm::vec4 colour;
	colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	Gizmos::addAABBFilled(glm::vec3(0, 0, 0), glm::vec3(10, 5, 10), colour, &m_project);

	Gizmos::draw(_camera->getProjectionView());
}