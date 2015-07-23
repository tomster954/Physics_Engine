//-------------------------------------
//Author tommas solarino
//description: this is a plane class
//-------------------------------------

#ifndef PLANE
#define PLANE

#include "glm/glm.hpp"

class Plane
{
public:
	Plane();
	~Plane();

	void Draw();

	void SetPosition(glm::vec3 _position){ m_transform[3].xyz = _position; }
	glm::vec3 GetNormal(){ return m_normal; }

	float GetBounce(){ return m_bounce; }

private:
	glm::mat4 m_transform;

	float m_bounce;

	glm::vec3 m_normal;

};
#endif