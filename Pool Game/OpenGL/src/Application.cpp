#include "Application.h"
#include "Camera.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

//Constructor
Application::Application(glm::vec4 _backgroundColour)
{
	if (glfwInit() == false)
		return;

	m_pWindow = glfwCreateWindow(1280, 720, "Checkers Game", nullptr, nullptr);

	m_camera = new Camera(3.14159f * 0.25f, 4.0f / 3.0f, 0.01f, 100);
	m_camera->setSpeed(100);
	m_camera->setLookAtFrom(glm::vec3(0, 300, -100), glm::vec3(0, 0, 0));

	if (m_pWindow == nullptr) 
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_pWindow);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) 
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
		return;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	glClearColor(_backgroundColour.r, _backgroundColour.g, _backgroundColour.b, _backgroundColour.a);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	m_currentTime	= 0.0f;
	m_deltaTime		= 0.0f;
	m_lastTime		= 0.0f;

	m_playState = new PlayState(m_pWindow);
}

//Destructor
Application::~Application()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void Application::RunGame()
{
	while (glfwWindowShouldClose(m_pWindow) == false &&
	glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS) 
	{
		Update();
		Draw();

		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();
	}
}

void Application::Update()
{
	m_currentTime	= (float)glfwGetTime();
	m_deltaTime		= m_currentTime - m_lastTime; // prev of last frame
	m_lastTime		= m_currentTime;

	m_camera->update(m_deltaTime);
	m_playState->Update(m_deltaTime);
}

void Application::Draw()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	m_playState->Draw(m_camera);
}