#include "Box.h"

#include "Gizmos.h"

Box::Box() :
	m_mass(10.0f),
	m_width(10.0f),
	m_height(10.0f),
	m_depth(10.0f),
	m_friction(10.0f),
	m_transform(1, 0, 0, 1,
				0, 1, 0, 1,
				0, 0, 1, 1,
				0, 0, 0, 1),
	m_velocity(0,0,0)
{

}

Box::~Box()
{

}

void Box::Update(float _dt)
{

}

void Box::Draw(Camera *_pCamera)
{
	glm::vec3 pos = m_transform[3].xyz;
	Gizmos::addAABBFilled(pos, glm::vec3(m_width, m_height, m_depth), m_colour, &m_transform);
}