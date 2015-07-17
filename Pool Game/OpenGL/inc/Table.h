//-------------------------------------
//Author: Tommas Solarino	
//Description: this is the entire pool table
//-------------------------------------

#ifndef TABLE
#define TABLE

#include "Box.h"
#include "Plane.h"
#include "Camera.h"

#include <array>

class Table
{
public:
	Table();
	~Table();

	void Update(float _dt);
	void Draw(Camera *_pCamera);
	
	std::array<Box, 6>* GetTableBorders(){ return &m_tableBarriers;  }
	Plane GetTable(){ return m_floor; }

private:
	glm::mat4 m_transform;
	Plane m_floor;

	std::array<Box, 6> m_tableBarriers;
};

#endif