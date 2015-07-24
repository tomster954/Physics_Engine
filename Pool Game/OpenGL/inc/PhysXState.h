//-------------------------------------
//Author: Tommas Solarino
//Description: this is the state that uses PhysX
//-------------------------------------

#ifndef PHYSXSTATE
#define PHYSXSTATE

#include "Camera.h"
struct GLFWwindow;

class PhysXState
{
public:
	PhysXState(GLFWwindow *_pWindow);
	~PhysXState();

	void Update(float _dt);
	void Draw(Camera *_camera);

private:

	GLFWwindow *m_pWindow;

	glm::mat4 m_project;
};

#endif