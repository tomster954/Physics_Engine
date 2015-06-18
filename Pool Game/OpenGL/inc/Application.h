//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Descriptio: The main application
//-----------------------------------------------------------------------------

#ifndef APPLICATION
#define APPLICATION

//Includes

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "PlayState.h"
#include "Networking.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

struct GLFWwindow;

class Camera;
class Application
{
public:
	//Constructor
	Application(vec4 _backgroundColour);
	//Destructor
	~Application();

	void RunGame();

	void Update();
	void Draw();

protected:

private:
	GLFWwindow *m_pWindow;
	Camera *m_camera;
	PlayState *m_playState;
	Networking *m_network;

	bool m_gameOver;

	float	m_lastTime;
	float	m_currentTime;
	float	m_deltaTime;
};

#endif
