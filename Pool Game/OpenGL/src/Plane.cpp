#include "Plane.h"
#include "Gizmos.h"
#include <limits>

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
	float size = 1000;

	glm::vec3 pos = m_transform[3].xyz;
	Gizmos::addAABBFilled(pos, glm::vec3(size, 0, size), glm::vec4(0.0f, 0.5f, 0.5f, 1), &m_transform);
}