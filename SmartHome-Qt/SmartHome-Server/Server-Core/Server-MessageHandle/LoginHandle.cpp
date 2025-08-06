#include "LoginHandle.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QImage>

#include "DataBaseQuery.h"
#include "ImageUtil.h"
#include "PacketCreate.h"
#include "LoginDBUtils.h"
#include "UserDBUtils.h"
#include "SResultCode.h"
#include "CreateId.h"

#include <jwt-cpp/jwt.h>
#include <picojson/picojson.h>


//登录认证
void LoginHandle::handle_loginValidation(const QJsonObject& paramsObj, const QByteArray& data, QHttpServerResponder& responder)
{
	auto user_id = paramsObj["user_id"].toString();
	auto password = paramsObj["password"].toString();
	//数据库查询
	DataBaseQuery query;
	if (!LoginDBUtils::validationPassWord(user_id, password, query))
	{
		QJsonObject allData;
		allData["type"] = "loginValidation";
		SResult::addFailuresCode(allData, SResultCode::UserLoginError);
		// 返回响应给客户端
		QJsonDocument responseDoc(allData);
		responder.write(responseDoc);
	}
	// 登录成功，生成 JWT 令牌
	try
	{
		// 生成 JWT 令牌
		auto token = jwt::create<jwt::traits::kazuho_picojson>()
			.set_issuer("auth0")
			.set_type("JWS")
			.set_payload_claim("user_id", jwt::claim(std::string(user_id.toStdString()))) // 将用户id作为 claim
			.sign(jwt::algorithm::hs256{ "xu-server" });

		// 创建响应数据，包含 token
		QJsonObject userObj;
		userObj["user_id"] = user_id;
		userObj["token"] = QString::fromStdString(token);
		QJsonObject allData;
		allData["type"] = "loginValidationSuccess";
		allData["params"] = userObj;
		SResult::addSuccessCode(allData, SResultCode::Success);
		// 返回响应给客户端
		QJsonDocument responseDoc(allData);
		responder.write(responseDoc);

	}
	catch (const std::exception& e)
	{
		// 如果生成 token 时发生异常，暂时不处理
		QJsonObject allData;
		allData["type"] = "loginValidationSuccess";
		SResult::addFailuresCode(allData, SResultCode::ServerSqlInnerError);
		// 返回响应给客户端
		QJsonDocument responseDoc(allData);
		responder.write(responseDoc);
	}

}

//登录
void LoginHandle::handle_login(const QJsonObject& paramsObj, const QByteArray& data, QHttpServerResponder& responder)
{
	auto user_id = paramsObj["user_id"].toString();
	auto password = paramsObj["password"].toString();
	//数据库查询
	DataBaseQuery query;
	QJsonObject responseObj = UserDBUtils::queryUserDetail(user_id, query);
	if (responseObj.contains("error"))
	{
		SResult::addFailuresCode(responseObj, SResultCode::ServerSqlQueryError);
		QJsonDocument responseDoc(responseObj);
		responder.write(responseDoc);
	}

	try
	{
		auto imageData = ImageUtils::loadImage(user_id);
		SResult::addSuccessCode(responseObj, SResultCode::Success);
		auto packet = PacketCreate::binaryPacket("loginSuccess", responseObj.toVariantMap(), imageData);
		QByteArray loginUserData;
		PacketCreate::addPacket(loginUserData, packet);
		auto allData = PacketCreate::allBinaryPacket(loginUserData);

		QByteArray mineType = "application/octet-stream";
		responder.write(allData, mineType);
	}
	catch (const std::exception& e)
	{
		SResult::addFailuresCode(responseObj, SResultCode::ServerSqlInnerError);
		// 返回响应给客户端
		QJsonDocument responseDoc(responseObj);
		responder.write(responseDoc);
	}


}

//注册
void LoginHandle::handle_register(const QJsonObject& paramsObj, const QByteArray& data, QHttpServerResponder& responder)
{
	RegisterMessage registerMessage;
	registerMessage.user_id = CreateId::generateUserID(CreateId::Id::User);
	registerMessage.username = paramsObj["user_name"].toString();
	registerMessage.password = paramsObj["password"].toString();
	registerMessage.confidential = paramsObj["confidential"].toString();
	//注册信息插入
	DataBaseQuery query;
	auto registerResult = query.executeTransaction([&](std::shared_ptr<QSqlQuery>queryPtr)->bool
		{
			if (!LoginDBUtils::insertUser(registerMessage, query, queryPtr))
			{
				return false;
			}
		});
	//注册成功后返回该用户账号与密码
	QJsonObject params;
	params["user_id"] = registerMessage.user_id;
	params["password"] = registerMessage.password;
	QJsonObject resgisterObj;
	resgisterObj["type"] = "registerSuccess";
	resgisterObj["params"] = params;
	if (registerResult)
		SResult::addSuccessCode(resgisterObj, SResultCode::Success);
	else
		SResult::addFailuresCode(resgisterObj, SResultCode::ServerSqlInnerError);

	QJsonDocument doc(resgisterObj);
	//http响应
	responder.write(doc);
}


