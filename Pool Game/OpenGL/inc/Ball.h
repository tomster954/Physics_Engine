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

	//Getters
	glm::mat4 GetTransform(){ return m_transform; }
	glm::vec3 GetVelocity(){ return m_velocity; }
	float GetRadius(){ return m_radius; }
	float GetMass(){ return m_mass; }

	//Setters
	void SetVelocity(glm::vec3 _velocity){ m_velocity = _velocity; }
	void SetPosition(glm::vec3 _position){ m_transform[3].xyz = _position; }
	void SetMass(float _mass){ m_mass = _mass; }

	//Add
	void ApplyForce(glm::vec3 _forceVector){ m_velocity += _forceVector / m_mass; }
	void AddPosition(glm::vec3 _pos){ m_transform[3].xyz += _pos; }

private:

	float m_mass;
	float m_radius;
	float m_drag;

	glm::mat4 m_transform;
	glm::vec3 m_velocity;
};
#endif