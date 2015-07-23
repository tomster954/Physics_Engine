#include "Table.h"
#include "Box.h"

#include "Gizmos.h"

Table::Table() :
	m_transform(1, 0, 0, 1,
				0, 1, 0, 1,
				0, 0, 1, 1,
				0, 0, 0, 1)
{
	//TopSide
	//---------------------------------
	m_tableBarriers[0].SetSize(glm::vec3(165, 10, 10));
	m_tableBarriers[0].SetPosition(glm::vec3(195, 10, 200));
	m_tableBarriers[0].SetColour(glm::vec4(0.5f, 0.2f, 0.1f, 1));
	m_tableBarriers[1].SetSize(glm::vec3(165, 10, 10));
	m_tableBarriers[1].SetPosition(glm::vec3(-195, 10, 200));
	m_tableBarriers[1].SetColour(glm::vec4(0.5f, 0.2f, 0.1f, 1));
	//---------------------------------

	//BotSide
	//---------------------------------
	m_tableBarriers[2].SetSize(glm::vec3(170, 10, 10));
	m_tableBarriers[2].SetPosition(glm::vec3(195, 10, -200));
	m_tableBarriers[2].SetColour(glm::vec4(0.5f, 0.2f, 0.1f, 1));
	m_tableBarriers[3].SetSize(glm::vec3(170, 10, 10));
	m_tableBarriers[3].SetPosition(glm::vec3(-195, 10,-200));
	m_tableBarriers[3].SetColour(glm::vec4(0.5f, 0.2f, 0.1f, 1));
	//---------------------------------

	//Left Side
	//---------------------------------
	m_tableBarriers[4].SetSize(glm::vec3(10, 10, 165));
	m_tableBarriers[4].SetPosition(glm::vec3(400, 10, 0));
	m_tableBarriers[4].SetColour(glm::vec4(0.5f, 0.2f, 0.1f, 1));
	//---------------------------------

	//right Side
	//---------------------------------
	m_tableBarriers[5].SetSize(glm::vec3(10, 10, 165));
	m_tableBarriers[5].SetPosition(glm::vec3(-400, 10, 0));
	m_tableBarriers[5].SetColour(glm::vec4(0.5f, 0.2f, 0.1f, 1));
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

	m_floor.Draw();
}