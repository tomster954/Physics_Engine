#include "Networking.h"
#include <stdio.h>
#include <string.h>

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

enum GameMessages
{
	ID_GAME_MESSAGE_CLIENTS_NAME = ID_USER_PACKET_ENUM + 1,
	ID_GAME_MESSAGE_WHOSE_TURN,
	ID_GAME_MESSAGE_WHO_MOVED_WHERE,

};

Networking::Networking()
{
	m_peer = RakNet::RakPeerInterface::GetInstance();
	m_serversTurn = false;

	SetUp();
}

Networking::~Networking()
{
	RakNet::RakPeerInterface::DestroyInstance(m_peer);
}
void Networking::ServerLoop()
{
	m_isThereAPieceToMove = false;
	m_shouldReset = false;

	for (m_packet = m_peer->Receive(); m_packet; m_peer->DeallocatePacket(m_packet), m_packet = m_peer->Receive())
	{
		m_systemAddress = m_packet->systemAddress;

		switch (m_packet->data[0])
		{
		case ID_GAME_MESSAGE_CLIENTS_NAME:
			{
				//When a Client joins it will say who joined in the server.
				RakNet::RakString rs;
				RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				printf("%s", rs.C_String());
				printf(" has joined the server\n");
			}break;
			case ID_GAME_MESSAGE_WHOSE_TURN:
			{
				//you have recived whos turn it is as a bool bluesTurn, if true its blues turn
				bool rs;
				RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				m_serversTurn = rs;
			}break;
			case ID_GAME_MESSAGE_WHO_MOVED_WHERE:
			{
				//you have recived a vec4 containing the grid location of the piece that moved and where it moved to.
				//The Start pos is x, y. The End pos is z, w.
				glm::vec4 rs;
				RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				m_startPos = glm::vec2(rs.x, rs.y);
				m_endPos = glm::vec2(rs.z, rs.w);
				m_isThereAPieceToMove = true;
			}break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf("Our connection request has been accepted.\n");
				//Reset the board if you get accepted into a game.
				m_shouldReset = true;
				//Send the Name of the client to the server.
				RakNet::BitStream nameOut;
				nameOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_CLIENTS_NAME);
				nameOut.Write(m_name);
				m_peer->Send(&nameOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_packet->systemAddress, false);
			}break;
			case ID_NEW_INCOMING_CONNECTION:
			{
				//Reset the board if there is an incomming connection
				m_shouldReset = true;
				printf("A connection is incoming.\n");
			}break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			{
				printf("Another client has disconnected.\n");
			}break;
			case ID_REMOTE_CONNECTION_LOST:
			{
				printf("Another client has lost the connection.\n");
			}break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
			{
				printf("Another client has connected.\n");
			}break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
			{
				printf("The server is full.\n");
			}break;
			case ID_DISCONNECTION_NOTIFICATION:
			{
				if (m_isServer)
				  printf("A client has disconnected.\n");
				else
				  printf("We have been disconnected.\n");
			}break;
			case ID_CONNECTION_LOST:
			{
				if (m_isServer)
					printf("A client lost the connection.\n");
				else
					printf("Connection lost.\n");
			}break;
			default:
			{
				printf("Message with identifier %i has arrived.\n", m_packet->data[0]);
			}break;
		}
	}
}


void Networking::SetUp()
{
	char str[512];

	printf("(C)onsole or (S)erver?\n");
	gets(str);

	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		RakNet::SocketDescriptor sd;
		m_peer->Startup(1, &sd, 1);
		m_isServer = false;
	}
	else {
		RakNet::SocketDescriptor sd(SERVER_PORT, 0);
		m_peer->Startup(MAX_CLIENTS, &sd, 1);
		m_isServer = true;
	}

	if (m_isServer)
	{
		printf("Starting the server.\n");
		// We need to let the server accept incoming connections from the clients
		m_peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	}
	else 
	{
		printf("Enter server IP or hit enter for 127.0.0.1\n");
		gets(str);
		if (str[0] == 0){
			strcpy(str, "127.0.0.1");
		}

		printf("Enter your name\n");
		gets(m_name);

		printf("Starting the client.\n");
		m_peer->Connect(str, SERVER_PORT, 0, 0);
	}
}

void Networking::SetWhosTurn(bool _whosTurnIsIt)
{
	m_serversTurn = _whosTurnIsIt;

	RakNet::BitStream whosTurn;
	whosTurn.Write((RakNet::MessageID)ID_GAME_MESSAGE_WHOSE_TURN);
	whosTurn.Write(_whosTurnIsIt);
	m_peer->Send(&whosTurn, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_systemAddress, false);
}

void Networking::SendPieceThatMoved(glm::vec2 _piecePos, glm::vec2 _endPos)
{
	glm::vec4 startAndEndPos = glm::vec4(_piecePos.x, _piecePos.y, _endPos.x, _endPos.y);

	RakNet::BitStream piecePos;
	piecePos.Write((RakNet::MessageID)ID_GAME_MESSAGE_WHO_MOVED_WHERE);
	piecePos.Write(startAndEndPos);
	m_peer->Send(&piecePos, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_systemAddress, false);
}