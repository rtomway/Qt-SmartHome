#include "UserDBUtils.h"
#include <QJsonArray>

//查看指定指定用户个人信息
QJsonObject UserDBUtils::queryUserDetail(const QString& user_id, DataBaseQuery& query, std::shared_ptr<QSqlQuery> queryPtr)
{
	QString queryStr = QString(
		"select f.Fgrouping,u.* from user u join friendship f \
		on (u.user_id=f.user_id) \
		where u.user_id=?"
	);
	QVariantList bindValues;
	bindValues.append(user_id);
	auto queryResult = query.executeQuery(queryStr, bindValues, queryPtr);
	QJsonObject queryUserObj;
	if (queryResult.contains("error"))
	{
		qDebug() << "queryUserDetail failed";
		queryUserObj["error"] = "";
		return queryUserObj;
	}
	QJsonArray dataArray = queryResult["data"].toArray();
	if (dataArray.isEmpty())
	{
		qDebug() << "queryUserDetail查询结果为空";
		return queryUserObj;
	}
	queryUserObj = dataArray.at(0).toObject();
	return queryUserObj;
}

//更新用户信息
bool UserDBUtils::updateUserMessage(const UserInfo& userInfo, DataBaseQuery& query, std::shared_ptr<QSqlQuery> queryPtr)
{
	QString queryStr = QString(
		"UPDATE user \
		SET username=?, gender=?, age=?, phone_number=?, \
		email=?, birthday=?, signature=? \
		WHERE user_id=?"
	);
	QVariantList bindValues;
	bindValues.append(userInfo.username);
	bindValues.append(userInfo.gender);
	bindValues.append(userInfo.age);
	// 使用 QVariant() 表示 NULL
	bindValues.append(userInfo.phone_number.isEmpty() ? QVariant(QVariant::String).toString() : userInfo.phone_number);
	bindValues.append(userInfo.email.isEmpty() ? QVariant(QVariant::String).toString() : userInfo.email);
	bindValues.append(userInfo.birthday);
	bindValues.append(userInfo.signature.isEmpty() ? QVariant(QVariant::String).toString() : userInfo.signature);
	bindValues.append(userInfo.user_id);

	return query.executeNonQuery(queryStr, bindValues, queryPtr);
}

//修改密码
bool UserDBUtils::passwordChange(const QString& user_id, const QString& password, DataBaseQuery& query, std::shared_ptr<QSqlQuery> queryPtr)
{
	QString queryStr = QString("UPDATE user	SET password=?	WHERE user_id=?");
	QVariantList bindValues;
	bindValues.append(password);
	bindValues.append(user_id);
	return query.executeNonQuery(queryStr, bindValues, queryPtr);
}
