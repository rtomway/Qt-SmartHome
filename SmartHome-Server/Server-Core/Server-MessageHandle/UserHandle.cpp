#include "UserHandle.h"
#include <QJsonArray>
#include <QJsonDocument>

#include "DataBaseQuery.h"
#include "ConnectionManager.h"
#include "ImageUtil.h"
#include "PacketCreate.h"
#include "UserDBUtils.h"

//查询user信息
void UserHandle::handle_queryUser(const QJsonObject& paramsObj, const QByteArray& data, QHttpServerResponder& responder)
{
	auto query_id = paramsObj["query_id"].toString();
	auto user_id = paramsObj["user_id"].toString();
	DataBaseQuery query;
	auto queryUserObj = UserDBUtils::queryUserDetail(query_id, query);
	if (queryUserObj.contains("error"))
	{

	}
	QByteArray userData;
	auto packet = PacketCreate::binaryPacket("queryUser", queryUserObj.toVariantMap(), QByteArray());
	PacketCreate::addPacket(userData, packet);
	auto allData = PacketCreate::allBinaryPacket(userData);
	QByteArray mimeType = "application/octet-stream";
	responder.write(allData, mimeType);
}

//用户信息更新
void UserHandle::handle_updateUserMessage(const QJsonObject& paramsObj, const QByteArray& data, QHttpServerResponder& responder)
{
	UserInfo userInfo;
	userInfo.user_id = paramsObj["user_id"].toString();
	userInfo.username = paramsObj["username"].toString();
	userInfo.gender = paramsObj["gender"].toInt();
	userInfo.age = paramsObj["age"].toInt();
	userInfo.phone_number = paramsObj["phone_number"].toString();
	userInfo.email = paramsObj["email"].toString();
	userInfo.birthday = QDateTime::fromString(paramsObj["birthday"].toString(), "MM-dd");
	userInfo.signature = paramsObj["signature"].toString();
	DataBaseQuery query;
	auto queryResult = UserDBUtils::updateUserMessage(userInfo, query);
	//错误返回
	if (!queryResult) {
		qDebug() << "Error query:";
		return;
	}
	responder.write(QHttpServerResponder::StatusCode::NoContent);
	
}

//用户修改密码
void UserHandle::handle_passwordChange(const QJsonObject& paramsObj, const QByteArray& data, QHttpServerResponder& responder)
{
	auto user_id = paramsObj["user_id"].toString();
	auto confidential = paramsObj["confidential"].toString();
	auto password = paramsObj["password"].toString();
	DataBaseQuery query;
	bool confidential_isRight = true;
	//验证并修改
	auto queryResult = query.executeTransaction([&](std::shared_ptr<QSqlQuery> queryPtr)->bool
		{
			auto personMessageObj = UserDBUtils::queryUserDetail(user_id, query, queryPtr);
			auto query_confidential = personMessageObj["confidential"].toString();
			if (query_confidential != confidential)
			{
				confidential_isRight = false;
				return false;
			}
			if (!UserDBUtils::passwordChange(user_id, password, query, queryPtr))
			{
				return false;
			}

		});

	// 创建响应数据，包含 token
	QJsonObject changeObj;

	//密保错误直接返回
	if (!queryResult && !confidential_isRight)
	{
		changeObj["error"] = true;
	}
	//服务器操作出错
	if (!queryResult && confidential_isRight)
	{
		changeObj["error"] = true;
	}
	if (queryResult && confidential_isRight)
	{
		changeObj["error"] = false;
	}
	// 返回响应给客户端
	QJsonObject allData;
	allData["type"] = "passwordChange";
	allData["params"] = changeObj;

	QJsonDocument responseDoc(allData);
	responder.write(responseDoc);

}

//用户头像更新 
void UserHandle::handle_updateUserAvatar(const QJsonObject& paramsObj, const QByteArray& data, QHttpServerResponder& responder)
{
	auto user_id = paramsObj["user_id"].toString();
	//将头像存储到服务器中
	QImage image;
	if (!image.loadFromData(data))  // 从二进制数据加载图片
	{
		qWarning() << "Failed to load avatar for user:" << user_id;
		return;
	}
	//图片保存
	ImageUtils::saveAvatarToLocal(image, user_id, ChatType::User);
	//转发头像信息
	//数据打包
	auto userPacket = PacketCreate::binaryPacket("updateUserAvatar", paramsObj.toVariantMap(), data);
	QByteArray userData;
	PacketCreate::addPacket(userData, userPacket);
	auto allData = PacketCreate::allBinaryPacket(userData);
}
