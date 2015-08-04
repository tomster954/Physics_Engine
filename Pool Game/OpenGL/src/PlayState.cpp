#include "PlayState.h"

#include <GLFW/glfw3.h>
#include <Gizmos.h>

#include "Camera.h"

#include <cmath>

PlayState::PlayState(GLFWwindow *_pWindow) :
	m_project(	1, 0, 0, 1,
				0, 1, 0, 1,
				0, 0, 1, 1,
				0, 0, 0, 1)
{
	Gizmos::create();
	m_pWindow = _pWindow;
	
	m_planeFloor.SetBounce(1.0f);

	ResetGame();
}

PlayState::~PlayState()
{

}

void PlayState::Update(float _dt)
{
	if (glfwGetKey(m_pWindow, GLFW_KEY_R) == GLFW_PRESS)
		ResetGame();

	CollisionDetection(_dt);

	//update all le balls 
	for (unsigned int i = 0; i < m_ballArray.size(); i++)
		m_ballArray[i].Update(_dt);
}

void PlayState::Draw(Camera *_camera)
{
	Gizmos::clear();
	glm::vec4 colour;
	colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	//Drawing balls
	for (unsigned int i = 0; i < m_ballArray.size(); i++)
		m_ballArray[i].Draw(_camera);

	//Drawing walls
	for (unsigned int i = 0; i < m_walls.size(); i++)
		m_walls[i].Draw(_camera);

	//Drawing the plane
	m_planeFloor.Draw();

	Gizmos::addTransform(glm::mat4(1), 20.0f);

	//Adding a line between the springy spheres
	Gizmos::addLine(m_ballArray[0].GetTransform()[3].xyz, m_ballArray[1].GetTransform()[3].xyz, colour);

	Gizmos::draw(_camera->getProjectionView());
}

void PlayState::ResetGame()
{
	//sets the walls height and mass
	for (unsigned int i = 0; i < m_walls.size(); i++)
	{
		m_walls[i].SetSize(glm::vec3(0, 30, 0));
		m_walls[i].SetColour(glm::vec4(0.5f, 1, 0.5f, 1));
	}

	//Positioning walls
	//----------------------------------------------------------------------------
	const int wallWidth = 1;
	const int wallLength = 100;
	const int wallHieght = m_walls[0].GetSzie().y / 2;

	m_walls[0].SetSize(glm::vec3(wallWidth, m_walls[0].GetSzie().y , wallLength));
	m_walls[0].SetPosition(glm::vec3(wallLength/2, wallHieght/2, 0));
	
	m_walls[1].SetSize(glm::vec3(wallWidth, m_walls[0].GetSzie().y, wallLength));
	m_walls[1].SetPosition(glm::vec3(-wallLength/2, wallHieght/2, 0));
	
	m_walls[2].SetSize(glm::vec3(wallLength, m_walls[0].GetSzie().y, wallWidth/2));
	m_walls[2].SetPosition(glm::vec3(0, wallHieght/2, wallLength/2));
	
	m_walls[3].SetSize(glm::vec3(wallLength, m_walls[0].GetSzie().y, wallWidth));
	m_walls[3].SetPosition(glm::vec3(0, wallHieght/2, -wallLength/2));
	//----------------------------------------------------------------------------

	//Arranging the balls
	//--------------------------------------------------------------------------
	float rndX, rndZ;
	for (int itr = 0; itr < m_ballArray.size(); itr++)
	{
		rndX = (float)((rand() % 800) - 400) / 10;
		rndZ = (float)((rand() % 800) - 400) / 10;

		m_ballArray[itr].SetPosition(glm::vec3(rndX, 10, rndZ));
		m_ballArray[itr].SetVelocity(glm::vec3(rndX * 2, 0, rndZ * 2));
		m_ballArray[itr].SetDrag(10.0f);
		m_ballArray[itr].SetColour(glm::vec4(0.9f, 1.0f, 0.5f, 1));
	}
	m_ballArray[0].SetColour(glm::vec4(1, 0.2f, 0, 1));
	m_ballArray[1].SetColour(glm::vec4(1, 0.2f, 0, 1));
	//----------------------------------------------------------------------------
}

void PlayState::CollisionDetection(float _dt)
{
	BallBallCollision();
	BallPlaneCollision();
	BallAABBCollision();
	Spring();
}

void PlayState::BallBallCollision()
{
	//Checking for collisions on all balls
	for (unsigned int x = 0; x < m_ballArray.size(); x++)
	{
		for (unsigned int y = 0; y < m_ballArray.size(); y++)
		{ 
			if (x == y)
				continue;

			Ball *ball1 = &m_ballArray[x];
			Ball *ball2 = &m_ballArray[y];

			glm::vec3 delta = ball2->GetTransform()[3].xyz - ball1->GetTransform()[3].xyz;
			float distance = glm::length(delta);
			float intersection = ball1->GetRadius() + ball2->GetRadius() - distance;

			if (intersection > 0)
			{
				glm::vec3 collisionNormal = glm::normalize(delta);
				glm::vec3 relativeVelocity = ball1->GetVelocity() - ball2->GetVelocity();
				glm::vec3 collisionVector = collisionNormal *(glm::dot(relativeVelocity, collisionNormal));
				glm::vec3 forceVector = collisionVector * 1.0f / (1 / ball1->GetMass() + 1 / ball2->GetMass());

				//use newtons third law to apply colision forces to colliding bodies.
				ball1->ApplyForce(2 * -forceVector);
				ball2->ApplyForce(2 * forceVector);
				//move balls out of collisions
				float fTotalMass = ball1->GetMass() + ball2->GetMass();

				glm::vec3 seperationVector = collisionNormal * intersection;
				ball1->AddPosition(-seperationVector * (ball1->GetMass() / fTotalMass));
				ball2->AddPosition(seperationVector* (ball2->GetMass() / fTotalMass));
			}
		}
	}
}

void PlayState::BallPlaneCollision()
{
	for (unsigned int x = 0; x < m_ballArray.size(); x++)
	{
		glm::vec3 planeNormal = m_planeFloor.GetNormal();
		glm::vec3 ballPosition = m_ballArray[x].GetTransform()[3].xyz;

		float sphereToPlane = glm::dot(ballPosition, planeNormal) - 0;

		if (sphereToPlane < 0)
		{
			planeNormal *= -1;
			sphereToPlane *= -1;
		}
		float intersection = m_ballArray[x].GetRadius() + 1 - sphereToPlane;
		if (intersection > 0)
		{
			glm::vec3 forceVec = -1 * m_ballArray[x].GetMass() * planeNormal * (glm::dot(planeNormal, m_ballArray[x].GetVelocity()));
			m_ballArray[x].ApplyForce(forceVec + forceVec * m_planeFloor.GetBounce());
			m_ballArray[x].SetPosition((m_ballArray[x].GetTransform()[3].xyz + planeNormal * intersection * 0.5f));
		}
	}
}

void PlayState::BallAABBCollision()
{
	for (unsigned int itrBall = 0; itrBall < m_ballArray.size(); itrBall++)
	{
		for (unsigned int itrWall = 0; itrWall < m_walls.size(); itrWall++)
		{
			bool colliding = false;
			glm::vec3 ballPos = m_ballArray[itrBall].GetTransform()[3].xyz;
			glm::vec3 wallPos = m_walls[itrWall].GetTransform()[3].xyz;
			float ballRadius = m_ballArray[itrBall].GetRadius();
			
			float halfWallWidth = m_walls[itrWall].GetExtents().x;
			float halfWallHeight = m_walls[itrWall].GetExtents().y;
			float halfWallLength = m_walls[itrWall].GetExtents().z;

			glm::vec3 wallTopBackLeft = wallPos - glm::vec3(halfWallWidth, halfWallHeight, -halfWallLength);
			glm::vec3 wallTopBackRight = wallPos - glm::vec3(-halfWallWidth, halfWallHeight, -halfWallLength);
			glm::vec3 wallTopFrontLeft = wallPos - glm::vec3(halfWallWidth, halfWallHeight, halfWallLength);
			glm::vec3 wallTopFrontRight = wallPos - glm::vec3(-halfWallWidth, halfWallHeight, halfWallLength);

			glm::vec3 wallBottomBackLeft = wallPos - glm::vec3(halfWallWidth, -halfWallHeight, -halfWallLength);
			glm::vec3 wallBottomBackRight = wallPos - glm::vec3(-halfWallWidth, -halfWallHeight, -halfWallLength);
			glm::vec3 wallBottomFrontLeft = wallPos - glm::vec3(halfWallWidth, -halfWallHeight, halfWallLength);
			glm::vec3 wallBottomFrontRight = wallPos - glm::vec3(-halfWallWidth, -halfWallHeight, halfWallLength);


			float	  fIntersectDist;
			glm::vec3 collisionNormal;
			float fXSmallestOverlap = 1000000;
			float fYSmallestOverlap = 1000000;
			float fZSmallestOverlap = 1000000;
			float fSmallestOverLap = 1000000;

			float fWallMinX = wallBottomFrontLeft.x;
			float fWallMaxX = wallBottomFrontRight.x;

			float fBallMinX = ballPos.x - ballRadius;
			float fBallMaxX = ballPos.x + ballRadius;

			if (fWallMaxX > fBallMinX && fBallMaxX > fWallMinX)
			{
				//Collided on X axis;
				float fXOverlap1 = fWallMaxX - fBallMinX;
				float fXOverLap2 = fWallMinX - fBallMaxX;
				if (glm::abs(fXOverlap1) < glm::abs(fXOverLap2))
				{
					fXSmallestOverlap = fXOverlap1;
				}
				else
				{
					fXSmallestOverlap = fXOverLap2;
				}
				collisionNormal = glm::vec3(1, 0, 0);
				fIntersectDist = fXSmallestOverlap;
				fSmallestOverLap = fXSmallestOverlap;
			}
			else
			{
				continue;
			}


			float fWallMinY = wallTopFrontLeft.y;
			float fWallMaxY = wallBottomBackRight.y;
						  
			float fBallMinY = ballPos.y - ballRadius;
			float fBallMaxY = ballPos.y + ballRadius;

			if (fWallMaxY > fBallMinY && fBallMaxY > fWallMinY)
			{
				//Collided on X axis;
				float fYOverlap1 = fWallMaxY - fBallMinY;
				float fYOverLap2 = fWallMinY - fBallMaxY;

				if (glm::abs(fYOverlap1) < glm::abs(fYOverLap2))
				{
					fYSmallestOverlap = fYOverlap1;
				}
				else
				{
					fYSmallestOverlap = fYOverLap2;
				}

				if (glm::abs(fYSmallestOverlap) < glm::abs(fSmallestOverLap))
				{
					collisionNormal = glm::vec3(0, 1, 0);
					fIntersectDist = fYSmallestOverlap;
					fSmallestOverLap = fYSmallestOverlap;
				}
			}
			else
			{
				continue;
			}

			float fWallMinZ = wallBottomFrontLeft.z;
			float fWallMaxZ = wallBottomBackRight.z;

			float fBallMinZ = ballPos.z - ballRadius;
			float fBallMaxZ = ballPos.z + ballRadius;

			if (fWallMaxZ > fBallMinZ && fBallMaxZ > fWallMinZ)
			{
				//Collided on X axis;
				float fZOverlap1 = fWallMaxZ - fBallMinZ;
				float fZOverLap2 = fWallMinZ - fBallMaxZ;

				if (glm::abs(fZOverlap1) < glm::abs(fZOverLap2))
				{
					fZSmallestOverlap = fZOverlap1;
				}
				else
				{
					fZSmallestOverlap = fZOverLap2;
				}

				if (glm::abs(fZSmallestOverlap) < glm::abs(fSmallestOverLap))
				{
					collisionNormal = glm::vec3(0, 0, 1);
					fIntersectDist = fZSmallestOverlap;
					fSmallestOverLap = fZSmallestOverlap;
				}
			}
			else
			{
				continue;
			}

			m_ballArray[itrBall].SetPosition((m_ballArray[itrBall].GetTransform()[3].xyz + collisionNormal * fIntersectDist));

			glm::vec3 forceVec = -1 * m_ballArray[itrBall].GetMass() * collisionNormal * (glm::dot(collisionNormal, m_ballArray[itrBall].GetVelocity()));
			m_ballArray[itrBall].ApplyForce(forceVec + forceVec);
		}
	}
}

void PlayState::Spring()
{
	glm::vec3 direction = m_ballArray[0].GetTransform()[3].xyz - m_ballArray[1].GetTransform()[3].xyz;
	float springCoeficient = 10;
	direction *= -springCoeficient;

	m_ballArray[0].ApplyForce(direction);
	m_ballArray[1].ApplyForce(-direction);

	Gizmos::addLine(m_ballArray[0].GetTransform()[3].xyz, m_ballArray[1].GetTransform()[3].xyz, glm::vec4(1, 1, 1, 1));
}