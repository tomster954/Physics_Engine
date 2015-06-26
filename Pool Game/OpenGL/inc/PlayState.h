//-----------------------------------------------------------------------------
//Description:: this is the play state. where the main game runs
//Author:: Tommas Solarino
//-----------------------------------------------------------------------------
#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include "Networking.h"
#include "Camera.h"
#include "Ball.h"
#include "Table.h"

#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>

struct GLFWwindow;

class PlayState
{
public:
	PlayState(GLFWwindow *_pWindow);
	~PlayState();
	void Update(float _dt);
	void Draw(Camera *_camera);
	void ResetGame();

	void BallCollision(Ball *_ball1, Ball *_ball2);

private:
	GLFWwindow *m_pWindow;
	glm::mat4 m_project;

	Table m_table;

	std::array<Ball, 16> m_ballArray;
};

#endif