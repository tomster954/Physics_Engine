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
	void Update(float _dt);
	void Draw(Camera *_camera);
	void ResetGame();

	void BallCollision();

private:
	GLFWwindow *m_pWindow;
	glm::mat4 m_project;

	Ball m_ball1;
	Ball m_ball2;
};

#endif