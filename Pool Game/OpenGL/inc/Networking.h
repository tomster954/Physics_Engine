//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Description:	this is the newtworking class, here is the code that send and 
//				recives all the nesseasry info from the other user
//-----------------------------------------------------------------------------

#ifndef NETWORKING
#define NETWORKING

#include "RakNetTypes.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Networking
{
public:

	Networking();
	~Networking();

	void ServerLoop();

	void SetUp();
	
	bool IsServer(){ return m_isServer; }
	void SetWhosTurn(bool _whosTurnIsIt);
	void SendPieceThatMoved(glm::vec2 _piecePos, glm::vec2 _endPos);

	bool BluesTurn(){ return m_serversTurn; }

	bool RecievedPieceToMove(){ return m_isThereAPieceToMove; }
	void RecievedPieceToMove(bool _bool){ m_isThereAPieceToMove = _bool; }

	glm::vec2 GetPieceToMove(){ return m_startPos; }
	glm::vec2 GetEndPos(){ return m_endPos; }

	bool ResetGame(){ return m_shouldReset; }

private:
	RakNet::Packet* m_packet;
	RakNet::RakPeerInterface *m_peer;

	RakNet::SystemAddress m_systemAddress;

	bool m_isServer;
	bool m_serversTurn;

	bool m_shouldReset;

	bool m_isThereAPieceToMove;
	
	glm::vec2 m_startPos;
	glm::vec2 m_endPos;

	char m_name[255];
};
#endif