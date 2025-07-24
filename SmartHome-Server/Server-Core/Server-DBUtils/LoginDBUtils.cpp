#include "LoginDBUtils.h"
#include <QJsonArray>

#include "ImageUtil.h"

//密码验证
bool LoginDBUtils::validationPassWord(const QString& user_id, const QString& password, DataBaseQuery& query, std::shared_ptr<QSqlQuery> queryPtr)
{
	QString queryStr = QString("select password from user where user_id=?");
	QVariantList bindValues;
	bindValues.append(user_id);
	auto queryResult = query.executeQuery(queryStr, bindValues);
	if (queryResult.contains("error"))
	{
		qDebug() << "validationPassWord failed";
		return false;
	}
	QJsonArray dataArray = queryResult["data"].toArray();
	if (dataArray.isEmpty()) 
	{
		qDebug() << "查询结果为空";
		return false;
	}
	auto passwordObj = dataArray.at(0).toObject();
	auto queryPassWord = passwordObj["password"].toString();
	if (queryPassWord == password)
	{
		return true;
	}
	return false;
}

//注册插入新用户
bool LoginDBUtils::insertUser(const RegisterMessage& registerMessage, DataBaseQuery& query, std::shared_ptr<QSqlQuery> queryPtr)
{
	QString queryStr = QString("insert into user (user_id,username,password,avatar_path,confidential)values(?,?,?,?,?)");
	auto avatarPath = ImageUtils::getUserAvatarFolderPath() + "/" + registerMessage.user_id + ".png";
	QVariantList bindValues;
	bindValues.append(registerMessage.user_id);
	bindValues.append(registerMessage.username);
	bindValues.append(registerMessage.password);
	bindValues.append(avatarPath);
	bindValues.append(registerMessage.confidential);
	auto insertResult = query.executeNonQuery(queryStr, bindValues,queryPtr);
	if (!insertResult)
	{
		qDebug() << "insertUser failed";
		return false;
	}
	return true;
}
