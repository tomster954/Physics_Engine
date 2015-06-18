//-----------------------------------------------------------------------------
//Description:: this is the play state. where the main game runs
//Author:: Tommas Solarino
//-----------------------------------------------------------------------------
#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include "Networking.h"
#include "Camera.h"
#include "Ball.h"

struct GLFWwindow;

class PlayState
{
public:
	PlayState(GLFWwindow *_pWindow);
	~PlayState();
	void Update();
	void Draw(Camera *_camera);
	void ResetGame();

private:
	GLFWwindow *m_pWindow;
	glm::mat4 *m_project;

	Ball *m_ball;
};

#endif