#include "UserDBUtils.h"
#include <QJsonArray>

//查看指定指定用户个人信息
QJsonObject UserDBUtils::queryUserDetail(const QString& user_id, DataBaseQuery& query, std::shared_ptr<QSqlQuery> queryPtr)
{
	QString queryStr = QString("select u.* from user u where u.user_id=?");
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



//修改密码
bool UserDBUtils::passwordChange(const QString& user_id, const QString& password, DataBaseQuery& query, std::shared_ptr<QSqlQuery> queryPtr)
{
	QString queryStr = QString("UPDATE user	SET password=?	WHERE user_id=?");
	QVariantList bindValues;
	bindValues.append(password);
	bindValues.append(user_id);
	return query.executeNonQuery(queryStr, bindValues, queryPtr);
}

//更新数据
bool UserDBUtils::insertData(int light, int temp, DataBaseQuery& query, std::shared_ptr<QSqlQuery> queryPtr)
{
	QString queryStr = QString("insert into data (light,temp)values(?,?)");
	QVariantList bindValues;
	bindValues.append(light);
	bindValues.append(temp);
	return query.executeNonQuery(queryStr, bindValues, queryPtr);
}
