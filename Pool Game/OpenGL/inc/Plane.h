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

	void Update(float _dt);
	void Draw();

	void SetSize(glm::vec2 _size){ m_size = _size; }
	void SetPosition(glm::vec3 _position){ m_transform[3].xyz = _position; }

private:
	glm::mat4 m_transform;
	glm::vec2 m_size;
	
	float m_width;
	float m_depth;
};
#endif