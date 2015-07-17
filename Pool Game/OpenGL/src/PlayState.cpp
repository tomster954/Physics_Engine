#include "PlayState.h"

#include <GLFW/glfw3.h>
#include <Gizmos.h>

#include "Camera.h"

PlayState::PlayState(GLFWwindow *_pWindow) :
	m_project(	1, 0, 0, 1,
				0, 1, 0, 1,
				0, 0, 1, 1,
				0, 0, 0, 1)
{
	Gizmos::create();
	m_pWindow = _pWindow;

	ResetGame();
}

PlayState::~PlayState()
{

}

void PlayState::Update(float _dt)
{
	if (glfwGetKey(m_pWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		ResetGame();

	CollisionDetection(_dt);

	//update all le balls 
	for (unsigned int i = 0; i < m_ballArray.size(); i++)
		m_ballArray[i].Update(_dt);

	m_table.Update(_dt);
}

void PlayState::Draw(Camera *_camera)
{
	Gizmos::clear();
	glm::vec4 colour;
	colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	Gizmos::addAABBFilled(glm::vec3(0, 0, 0), glm::vec3(10, 5, 10), colour, &m_project);

	for (unsigned int i = 0; i < m_ballArray.size(); i++)
		m_ballArray[i].Draw(_camera);

	m_table.Draw(_camera);

	Gizmos::draw(_camera->getProjectionView());
}

void PlayState::ResetGame()
{
	//Sets all the velocities to 0 and mass to 10
	for (unsigned int i = 0; i < m_ballArray.size(); i++)
	{
		m_ballArray[i].SetVelocity(glm::vec3(0, 0, 0));
 		m_ballArray[i].SetMass(10);
	}

	//white ball
	m_ballArray[0].SetVelocity(glm::vec3(-300, 0, 0));
	m_ballArray[0].SetPosition(glm::vec3(200, 10, 0));
	

	//placess all the pool balls
	int ballNumber = 1;
	int ballDiameter = 20;
	for (int col = 1; col < 6; col++)
	{
		float x = (-ballDiameter + 2.5f) * col;

		for (int  i = 0; i < col; i++)
		{
			int z = (ballDiameter * col) / 2 - (ballDiameter / 2);
			z += -ballDiameter * i;

			m_ballArray[ballNumber].SetPosition(glm::vec3(x - 100, 80, z));
			ballNumber++;
		}
	}
}

void PlayState::CollisionDetection(float _dt)
{
	BallBallCollision();
	BallPlaneCollision();
}

void PlayState::BallBallCollision()
{
	//Checking for collisions on all balls
	for (unsigned int x = 0; x < m_ballArray.size(); x++)
	{
		for (unsigned int y = 0; y < m_ballArray.size(); y++)
		{
			if (x == y)
				continue;

			Ball *ball1 = &m_ballArray[x];
			Ball *ball2 = &m_ballArray[y];

			glm::vec3 delta = ball2->GetTransform()[3].xyz - ball1->GetTransform()[3].xyz;
			float distance = glm::length(delta);
			float intersection = ball1->GetRadius() + ball2->GetRadius() - distance;

			if (intersection > 0)
			{
				glm::vec3 collisionNormal = glm::normalize(delta);
				glm::vec3 relativeVelocity = ball1->GetVelocity() - ball2->GetVelocity();
				glm::vec3 collisionVector = collisionNormal *(glm::dot(relativeVelocity, collisionNormal));
				glm::vec3 forceVector = collisionVector * 1.0f / (1 / ball1->GetMass() + 1 / ball2->GetMass());

				//use newtons third law to apply colision forces to colliding bodies.
				ball1->ApplyForce(2 * -forceVector);
				ball2->ApplyForce(2 * forceVector);
				//move balls out of collisions
				float fTotalMass = ball1->GetMass() + ball2->GetMass();

				glm::vec3 seperationVector = collisionNormal * intersection;
				ball1->AddPosition(-seperationVector * (ball1->GetMass() / fTotalMass));
				ball2->AddPosition(seperationVector* (ball2->GetMass() / fTotalMass));
			}
		}
	}
}

void PlayState::BallPlaneCollision()
{
	for (unsigned int x = 0; x < m_ballArray.size(); x++)
	{
		//Todo check against the plane.

		if (abs(m_ballArray[x].GetTransform()[3].y) < 50)
		{
			glm::vec3 planeNormal = m_table.GetTable().GetNormal();
			glm::vec3 ballPosition = m_ballArray[x].GetTransform()[3].xyz;

			float sphereToPlane = glm::dot(ballPosition, planeNormal) - 0;

			if (sphereToPlane < 0)
			{
				planeNormal *= -1;
				sphereToPlane *= -1;
			}
			float intersection = m_ballArray[x].GetRadius() + 1 - sphereToPlane;
			if (intersection > 0)
			{
				glm::vec3 forceVec = -1 * m_ballArray[x].GetMass() * planeNormal * (glm::dot(planeNormal, m_ballArray[x].GetVelocity()));
				m_ballArray[x].ApplyForce(forceVec + forceVec * m_table.GetTable().GetBounce());
				m_ballArray[x].SetPosition((m_ballArray[x].GetTransform()[3].xyz + planeNormal * intersection * 0.5f));
			}
		}
	}
}