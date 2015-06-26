#include "Table.h"
#include "Box.h"

#include "Gizmos.h"

Table::Table() :
	m_transform(1, 0, 0, 1,
				0, 1, 0, 1,
				0, 0, 1, 1,
				0, 0, 0, 1)
{
	m_floor.SetSize(glm::vec3(400, 1, 200));
	m_floor.SetMass(1000000);

	//TopSide
	//---------------------------------
	m_tableBarriers[0].SetSize(glm::vec3(175, 10, 10));
	m_tableBarriers[1].SetSize(glm::vec3(175, 10, 10));
	m_tableBarriers[0].SetPosition(glm::vec3(200, 10, 200));
	m_tableBarriers[1].SetPosition(glm::vec3(-200, 10, 200));
	//---------------------------------

	//BotSide
	//---------------------------------
	m_tableBarriers[2].SetSize(glm::vec3(175, 10, 10));
	m_tableBarriers[3].SetSize(glm::vec3(175, 10, 10));
	m_tableBarriers[2].SetPosition(glm::vec3(200, 10, -200));
	m_tableBarriers[3].SetPosition(glm::vec3(-200, 10,-200));
	//---------------------------------

	//Left Side
	//---------------------------------
	m_tableBarriers[4].SetSize(glm::vec3(10, 10, 175));
	m_tableBarriers[4].SetPosition(glm::vec3(400, 10, 0));
	//---------------------------------

	//right Side
	//---------------------------------
	m_tableBarriers[5].SetSize(glm::vec3(10, 10, 175));
	m_tableBarriers[5].SetPosition(glm::vec3(-400, 10, 0));
	//---------------------------------

}

Table::~Table()
{

}

void Table::Update(float _dt)
{

}

void Table::Draw(Camera *_pCamera)
{
	for (unsigned int i = 0; i < m_tableBarriers.size(); i++)
		m_tableBarriers[i].Draw(_pCamera);

	m_floor.Draw(_pCamera);
	//glm::vec3 pos = m_transform[3].xyz;
	//Gizmos::addAABBFilled(pos, glm::vec3(400, 1, 200), glm::vec4(0.0f, 0.8f, 0.2f, 1), &m_transform);
}