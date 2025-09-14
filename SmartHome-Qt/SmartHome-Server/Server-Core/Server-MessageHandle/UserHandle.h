#ifndef USERHANDLE_H_
#define USERHANDLE_H_

#include <QJsonObject>
#include <QByteArray>
#include <QHttpServerResponder>

class UserHandle
{
public:
	static void handle_updateUserAvatar(const QJsonObject& paramsObj, const QByteArray& data, QHttpServerResponder& responder);
	static void handle_passwordChange(const QJsonObject& paramsObj, const QByteArray& data, QHttpServerResponder& responder);
	static void handle_dataPost(const QJsonObject& paramsObj, const QByteArray& data, QHttpServerResponder& responder);
};

#endif // !USERHANDLE_H_