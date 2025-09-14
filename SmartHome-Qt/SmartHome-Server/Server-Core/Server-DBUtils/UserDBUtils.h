#ifndef USERDBUTILS_H_
#define USERDBUTILS_H_

#include <QSqlQuery>
#include "DataBaseQuery.h"

struct UserInfo {
	QString user_id;
	QString username;
	int gender;
	int age;
	QString phone_number;
	QString email;
	QDateTime birthday;
	QString signature;
};


class UserDBUtils
{
public:
	static QJsonObject queryUserDetail(const QString& user_id, DataBaseQuery& query, std::shared_ptr<QSqlQuery> queryPtr = nullptr);
	static bool updateUserMessage(const UserInfo& userInfo, DataBaseQuery& query, std::shared_ptr<QSqlQuery> queryPtr = nullptr);
	static bool passwordChange(const QString& user_id, const QString& password, DataBaseQuery& query, std::shared_ptr<QSqlQuery> queryPtr = nullptr);
	static bool insertData(int light,int temp, DataBaseQuery& query, std::shared_ptr<QSqlQuery> queryPtr = nullptr);
};

#endif // !USERDBUTILS_H_
