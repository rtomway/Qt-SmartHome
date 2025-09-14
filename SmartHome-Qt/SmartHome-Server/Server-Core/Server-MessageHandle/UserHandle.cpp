#include "UserHandle.h"
#include <QJsonArray>
#include <QJsonDocument>

#include "DataBaseQuery.h"
#include "ImageUtil.h"
#include "PacketCreate.h"
#include "UserDBUtils.h"
#include "SResultCode.h"

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

	// 返回响应给客户端
	QJsonObject allData;
	allData["type"] = "passwordChange";
	allData["params"] = "";
	//密保错误直接返回
	if (!queryResult && !confidential_isRight)
	{
		SResult::addFailuresCode(allData, SResultCode::UserConfidentialError);
	}
	//服务器操作出错
	if (!queryResult && confidential_isRight)
	{
		SResult::addFailuresCode(allData, SResultCode::ServerSqlInnerError);
	}
	if (queryResult && confidential_isRight)
	{
		SResult::addFailuresCode(allData, SResultCode::Success);
	}

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
	ImageUtils::saveAvatarToLocal(image, user_id);

	QJsonObject responseObj;
	SResult::addSuccessCode(responseObj, SResultCode::Success);

	QJsonDocument responsedoc(responseObj);
	responder.write(responsedoc);
}

//数据上传
void UserHandle::handle_dataPost(const QJsonObject& paramsObj, const QByteArray& data, QHttpServerResponder& responder)
{
	auto light = paramsObj["light"].toString().toInt();
	auto temp = paramsObj["temp"].toString().toInt();
	DataBaseQuery query;
	if (!UserDBUtils::insertData(light, temp, query))
	{

	}
	QJsonObject responseObj;
	SResult::addSuccessCode(responseObj, SResultCode::SuccessButNotData);

	QJsonDocument responsedoc(responseObj);
	responder.write(responsedoc);

}
