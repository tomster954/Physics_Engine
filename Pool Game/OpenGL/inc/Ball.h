//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Decription: This is a pool ball.
//-----------------------------------------------------------------------------

#ifndef BALL
#define BALL

#include "Camera.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

class Ball
{
public:
	Ball();
	~Ball();

	void Update(float _dt);
	void Draw(Camera *_pCamera);

private:

	float m_mass;
	float m_radius;
	float m_velocity;

	glm::mat4 m_transform;
};
#endif