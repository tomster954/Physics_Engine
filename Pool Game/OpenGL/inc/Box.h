//-------------------------------------
//Author: Tommas Solarino
//description: this is a box class that u can use to create boxes
//-------------------------------------

#ifndef BOX
#define BOX
#include "Camera.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

class Box
{
public:
	Box();
	~Box();
	void Update(float _dt);
	void Draw(Camera *_pCamera);
	
	void SetSize(glm::vec3 _size){	m_width = _size.x; 
									m_height = _size.y; 
									m_depth = _size.z; };
	void SetMass(float _mass){ m_mass = _mass; }
	void SetPosition(glm::vec3 _pos) { m_transform[3].xyz = _pos; }
	void SetColour(glm::vec4 _colour) { m_colour = _colour; }
private:
	float m_mass;

	float m_width;
	float m_height;
	float m_depth;

	float m_friction;

	
	glm::vec4 m_colour;
	glm::mat4 m_transform;
	glm::vec3 m_velocity;

};

#endif