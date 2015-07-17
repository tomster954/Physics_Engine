#include "Plane.h"
#include "Gizmos.h"

Plane::Plane() : 
m_transform(1, 0, 0, 1,
			0, 1, 0, 1,
			0, 0, 1, 1,
			0, 0, 0, 1),
m_normal(0, 1, 0),
m_bounce(0.3f)

{

}

Plane::~Plane()
{

	
}

void Plane::Draw()
{
	glm::vec3 pos = m_transform[3].xyz;
	Gizmos::addAABBFilled(pos, glm::vec3(m_size.x, 1, m_size.y), glm::vec4(0.0f, 0.8f, 0.2f, 1), &m_transform);
}

void Plane::Update(float _dt)
{

}