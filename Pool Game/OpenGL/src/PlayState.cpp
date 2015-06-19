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
	/*for(int x = 0; x < maxObject; x++){
		for(int y = 0; y < maxObjects; y++)
		{
			if(x != y && GetChecked(x, y) == false)
			{
				CheckCollision(x, y);
			}
		}
	}

	for
	*/

	m_ball1.Update(_dt);
	m_ball2.Update(_dt);

	BallCollision();
}

void PlayState::Draw(Camera *_camera)
{
	Gizmos::clear();
	glm::vec4 colour;
	colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	Gizmos::addAABBFilled(glm::vec3(0, 0, 0), glm::vec3(10, 5, 10), colour, &m_project);

	m_ball1.Draw(_camera);
	m_ball2.Draw(_camera);

	Gizmos::draw(_camera->getProjectionView());
}

void PlayState::ResetGame()
{
	m_ball1 = Ball();
	m_ball2 = Ball();

	m_ball1.SetVelocity(glm::vec3(-150, 0, 0));

	m_ball1.SetPosition(glm::vec3(100, 0, 10));
	m_ball2.SetPosition(glm::vec3(0, 0, 0));

	m_ball1.SetMass(10);
	m_ball2.SetMass(10);
}

void PlayState::BallCollision()
{
	glm::vec3 delta = m_ball2.GetTransform()[3].xyz - m_ball1.GetTransform()[3].xyz;
	float distance = glm::length(delta);
	float intersection = m_ball1.GetRadius() + m_ball2.GetRadius() - distance;

	if (intersection > 0)
	{
		glm::vec3 collisionNormal = glm::normalize(delta);
		glm::vec3 relativeVelocity = m_ball1.GetVelocity() - m_ball2.GetVelocity();
		glm::vec3 collisionVector = collisionNormal *(glm::dot(relativeVelocity, collisionNormal));
		glm::vec3 forceVector = collisionVector * 1.0f / (1 / m_ball1.GetMass() + 1 / m_ball2.GetMass());
		
		//glm::vec3 fTotalMomentum = relativeVelocity * m_ball2.GetMass();
		//use newtons third law to apply colision forces to colliding bodies.
		m_ball1.ApplyForce(2 * -forceVector);		m_ball2.ApplyForce( 2 * forceVector);
		//move balls out of collisions
		float fTotalMass = m_ball1.GetMass() + m_ball2.GetMass();

		glm::vec3 seperationVector = collisionNormal * intersection;
		m_ball1.AddPosition(-seperationVector * (m_ball1.GetMass() / fTotalMass));
		m_ball2.AddPosition(seperationVector* (m_ball2.GetMass() / fTotalMass));
	}
}