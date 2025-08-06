#ifndef SERVER_H_
#define  SERVER_H_

#include <QHttpServer>
#include "HttpRouteManager.h"

class Server :public QObject
{
	Q_OBJECT
public:
	explicit Server(QObject* parent = nullptr);
	~Server();
private:
	QHttpServer* m_httpServer{};
	HttpRouteManager* m_httpRouteManager{};
};

#endif // !SERVER_H_
