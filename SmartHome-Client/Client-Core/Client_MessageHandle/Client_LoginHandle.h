#ifndef CLIENT_LOGINHANDLE_H_
#define CLIENT_LOGINHANDLE_H_

#include <QJsonObject>

class Client_LoginHandle
{
public:
	static void handle_loginValidationSuccess(const QJsonObject& paramsObject = QJsonObject(), const QByteArray& data = QByteArray());
	static void handle_loginSuccess(const QJsonObject& paramsObject = QJsonObject(), const QByteArray& data = QByteArray());
	static void handle_registerSuccess(const QJsonObject& paramsObject = QJsonObject(), const QByteArray& data = QByteArray());
signals:
	void connectToServer();
};

#endif // !CLIENT_LOGINHANDLE_H_