#include "Ball.h"
#include "Gizmos.h"
#include "glm/ext.hpp"
Ball::Ball() : 
	m_transform(1, 0, 0, 1,
				0, 1, 0, 1,
				0, 0, 1, 1,
				0, 30, 0, 1), 
	m_velocity(0),
	m_radius(2),
	m_mass(100),
	m_drag(20),
	m_colour(1.0f, 1.0f, 1.0f, 1.0f)
{
}

Ball::~Ball()
{
}

void Ball::Update(float _dt)
{
	glm::vec3 gravity = glm::vec3(0, 1, 0);

	m_velocity -= gravity * m_mass * _dt;

	//Calculating drag
	glm::vec3 normalVelocity(0);
	if (m_velocity != glm::vec3(0))
		normalVelocity = glm::normalize(m_velocity);

	m_velocity -= normalVelocity * m_drag * _dt;

	//Transforming the ball with the velocity
	m_transform[3].xyz = glm::vec3(	m_transform[3].x + (m_velocity.x * _dt),
									m_transform[3].y + (m_velocity.y * _dt),
									m_transform[3].z + (m_velocity.z * _dt));
}

void Ball::Draw(Camera *_pCamera)
{
	glm::vec3 pos = m_transform[3].xyz;
	Gizmos::addSphere(pos, m_radius, 10, 10, m_colour);
}