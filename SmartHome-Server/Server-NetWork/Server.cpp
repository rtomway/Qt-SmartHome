#include "Server.h"


Server::Server(QObject* parent)
	:QObject(parent)
{

	//http服务器
	if (!m_httpServer)
	{
		m_httpServer = new QHttpServer(this);
	}
	m_httpServer->listen(QHostAddress::Any, 8889);
	m_httpRouteManager = new HttpRouteManager(m_httpServer);
}

Server::~Server()
{
	
}





