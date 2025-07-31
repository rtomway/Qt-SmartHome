#ifndef LOGINHANDLE_H_
#define LOGINHANDLE_H_

#include <QJsonObject>
#include <QByteArray>
#include <QHttpServerResponder>

class LoginHandle
{
public:
	static void handle_login(const QJsonObject& paramsObj, const QByteArray& data, QHttpServerResponder& responder);
	static void handle_loginValidation(const QJsonObject& paramsObj, const QByteArray& data, QHttpServerResponder& responder);
	static void handle_register(const QJsonObject& paramsObj, const QByteArray& data, QHttpServerResponder& responder);
};

#endif // !LOGINHANDLE_H_