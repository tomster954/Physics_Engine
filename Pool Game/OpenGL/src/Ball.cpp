#include "Ball.h"

#include "Gizmos.h"

Ball::Ball() :
	m_transform(1, 0, 0, 1,
			  0, 1, 0, 1,
			  0, 0, 1, 1,
			  0, 30, 0, 1)
{
}

Ball::~Ball()
{
}

void Ball::Update(float _dt)
{

}

void Ball::Draw(Camera *_pCamera)
{
	glm::vec3 pos;

	pos = m_transform[3].xyz;

	Gizmos::addSphere(pos, 10, 10, 10, glm::vec4(0.25, 1, 1, 1), &m_transform);
}