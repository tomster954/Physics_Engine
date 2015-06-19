#include "Ball.h"
#include "Gizmos.h"

Ball::Ball() : 
	m_transform(1, 0, 0, 1,
				0, 1, 0, 1,
				0, 0, 1, 1,
				0, 30, 0, 1), 
	m_velocity(0),
	m_radius(10),
	m_mass(100),
	m_friction(20)
{


}

Ball::~Ball()
{
}

void Ball::Update(float _dt)
{
	//slows ball over time.

	for (int i = 0; i < 3; i++)
	{
		if (m_velocity[i] > 0.5)
			m_velocity[i] -= m_friction * _dt;
		else if (m_velocity[i] < -0.5)
			m_velocity[i] += m_friction * _dt;
		else
			m_velocity[i] = 0;
	}

	float speed = 10;

	m_transform[3].xyz = glm::vec3(	m_transform[3].x + (m_velocity.x * _dt),
									m_transform[3].y + (m_velocity.y * _dt),
									m_transform[3].z + (m_velocity.z * _dt));
}

void Ball::Draw(Camera *_pCamera)
{
	glm::vec3 pos;

	pos = m_transform[3].xyz;

	Gizmos::addSphere(pos, 10, 10, 10, glm::vec4(0.25, 1, 1, 1), &m_transform);
}