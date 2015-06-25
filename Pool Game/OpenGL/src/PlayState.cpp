#include "PlayState.h"

#include <GLFW/glfw3.h>
#include <Gizmos.h>

#include "Camera.h"

PlayState::PlayState(GLFWwindow *_pWindow)
{
	Gizmos::create();
	m_pWindow = _pWindow;
	
	m_project = glm::mat4(1, 0, 0, 1,
							0, 1, 0, 1,
							0, 0, 1, 1,
							0, 0, 0, 1);

	ResetGame();
}

PlayState::~PlayState()
{

}

void PlayState::Update(float _dt)
{
	if (glfwGetKey(m_pWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		ResetGame();
	}

	for(int x = 0; x < m_ballArray.size(); x++)
	{
		for (int y = 0; y < m_ballArray.size(); y++)
		{
			if(x != y)
			{
				BallCollision(&m_ballArray[x], &m_ballArray[y]);
			}
		}
	}

	//update all le balls 
	for (int i = 0; i < m_ballArray.size(); i++)
		m_ballArray[i].Update(_dt);
}

void PlayState::Draw(Camera *_camera)
{
	Gizmos::clear();
	glm::vec4 colour;
	colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	Gizmos::addAABBFilled(glm::vec3(0, 0, 0), glm::vec3(10, 5, 10), colour, &m_project);

	for (int i = 0; i < m_ballArray.size(); i++)
		m_ballArray[i].Draw(_camera);

	Gizmos::draw(_camera->getProjectionView());
}

void PlayState::ResetGame()
{
	//Sets all the velocities to 0 and mass to 10
	for (int i = 0; i < m_ballArray.size(); i++)
	{
		m_ballArray[i].SetVelocity(glm::vec3(0, 0, 0));
 		m_ballArray[i].SetMass(10);
	}

	//white ball
	m_ballArray[0].SetVelocity(glm::vec3(-150, 0, 0));
	m_ballArray[0].SetPosition(glm::vec3(100, 0, 0));
	
	int ballNumber = 1;
	int ballDiameter = 20;
	for (int col = 1; col < 6; col++)
	{
		float x = (-ballDiameter + 2.5f) * col;

		for (int  i = 0; i < col; i++)
		{
			int z = (ballDiameter * col) / 2 - (ballDiameter / 2);
			z += -ballDiameter * i;

			m_ballArray[ballNumber].SetPosition(glm::vec3(x, 0, z));
			ballNumber++;
		}
	}
}

void PlayState::BallCollision(Ball *_ball1, Ball *_ball2)
{
	glm::vec3 delta = _ball2->GetTransform()[3].xyz - _ball1->GetTransform()[3].xyz;
	float distance = glm::length(delta);
	float intersection = _ball1->GetRadius() + _ball2->GetRadius() - distance;

	if (intersection > 0)
	{
		glm::vec3 collisionNormal = glm::normalize(delta);
		glm::vec3 relativeVelocity = _ball1->GetVelocity() - _ball2->GetVelocity();
		glm::vec3 collisionVector = collisionNormal *(glm::dot(relativeVelocity, collisionNormal));
		glm::vec3 forceVector = collisionVector * 1.0f / (1 / _ball1->GetMass() + 1 / _ball2->GetMass());
		
		//glm::vec3 fTotalMomentum = relativeVelocity * m_ball2.GetMass();
		//use newtons third law to apply colision forces to colliding bodies.
		_ball1->ApplyForce(2 * -forceVector);
		_ball2->ApplyForce(2 * forceVector);
		//move balls out of collisions
		float fTotalMass = _ball1->GetMass() + _ball2->GetMass();

		glm::vec3 seperationVector = collisionNormal * intersection;
		_ball1->AddPosition(-seperationVector * (_ball1->GetMass() / fTotalMass));
		_ball2->AddPosition(seperationVector* (_ball2->GetMass() / fTotalMass));
	}
}