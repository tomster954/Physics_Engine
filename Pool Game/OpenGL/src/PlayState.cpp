#include "PlayState.h"

#include <GLFW/glfw3.h>
#include <Gizmos.h>

#include "Camera.h"

PlayState::PlayState(GLFWwindow *_pWindow)
{
	Gizmos::create();
	m_pWindow = _pWindow;
	
	ResetGame();

	m_project = new glm::mat4(	1, 0, 0, 1,
								0, 1, 0, 1,
								0, 0, 1, 1,
								0, 0, 0, 1);

	m_ball = new Ball();
}

PlayState::~PlayState()
{
}

void PlayState::Update()
{
}

void PlayState::Draw(Camera *_camera)
{
	Gizmos::clear();
	glm::vec4 colour;
	colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	Gizmos::addAABBFilled(glm::vec3(0, 0, 0), glm::vec3(10, 5, 10), colour, m_project);

	m_ball->Draw(_camera);

	Gizmos::draw(_camera->getProjectionView());
}

void PlayState::ResetGame()
{
}