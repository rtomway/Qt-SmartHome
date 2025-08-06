#ifndef LOGINDBUTILS_H_
#define LOGINDBUTILS_H_

#include <QSqlQuery>
#include <QJsonObject>

#include "DataBaseQuery.h"

struct RegisterMessage
{
	QString user_id;
	QString username;
	QString password;
	QString confidential;
};

class LoginDBUtils
{
public:
	static bool validationPassWord(const QString& user_id, const QString& password, DataBaseQuery& query, std::shared_ptr<QSqlQuery> queryPtr = nullptr);
	static bool insertUser(const RegisterMessage& registerMessage, DataBaseQuery& query, std::shared_ptr<QSqlQuery> queryPtr = nullptr);
};

#endif // !LOGINDBUTILS_H_
