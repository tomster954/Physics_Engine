#include "Plane.h"
#include "Gizmos.h"

Plane::Plane()
{
}

Plane::~Plane()
{

	glm::vec3 pos = m_transform[3].xyz;
	Gizmos::addAABBFilled(pos, glm::vec3(m_width, 1, m_depth), glm::vec4(0.0f, 0.8f, 0.2f, 1), &m_transform);
}

void Plane::Draw()
{

}

void Plane::Update(float _dt)
{

}