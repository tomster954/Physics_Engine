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

	void CollisionDetection(float _dt);

	void BallBallCollision();
	void BallPlaneCollision();
	void BallAABBCollision();
	void Spring();

private:
	GLFWwindow *m_pWindow;
	glm::mat4 m_project;

	Plane m_planeFloor;

	std::array<Box, 4> m_walls;
	std::array<Ball, 50> m_ballArray;
};

#endif