#include "LoginHandle.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QImage>

#include "DataBaseQuery.h"
#include "ConnectionManager.h"
#include "ImageUtil.h"
#include "PacketCreate.h"
#include "LoginDBUtils.h"
#include "UserDBUtils.h"

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

	}
	QJsonObject userObj;
	userObj["user_id"] = user_id;
	// 登录成功，生成 JWT 令牌
	try {
		// 生成 JWT 令牌
		auto token = jwt::create<jwt::traits::kazuho_picojson>()  // 使用 kazuho-picojson traits
			.set_issuer("auth0")
			.set_type("JWS")
			.set_payload_claim("user_id", jwt::claim(std::string(user_id.toStdString()))) // 将用户名作为 claim
			.sign(jwt::algorithm::hs256{ "xu-server" });

		std::cout << "=== JWT Token ===" << std::endl;
		std::cout << token << std::endl << std::endl;

		userObj["token"] = QString::fromStdString(token);
		// 创建响应数据，包含 token
		QJsonObject allData;
		allData["type"] = "loginValidationSuccess";
		allData["params"] = userObj;
		// 返回响应给客户端
		QJsonDocument responseDoc(allData);
		responder.write(responseDoc);

	}
	catch (const std::exception& e) {
		// 如果生成 token 时发生异常，暂时不处理
	}

}

//登录
void LoginHandle::handle_login(const QJsonObject& paramsObject, const QByteArray& data)
{
	auto user_id = paramsObject["user_id"].toString();
	auto password = paramsObject["password"].toString();
	//数据库查询
	DataBaseQuery query;
	auto loginUserObj = UserDBUtils::queryUserDetail(user_id, query);
	if (loginUserObj.contains("error"))
	{

	}
	auto imageData = ImageUtils::loadImage(user_id, ChatType::User);
	auto packet = PacketCreate::binaryPacket("loginSuccess", loginUserObj.toVariantMap(), imageData);
	QByteArray loginUserData;
	PacketCreate::addPacket(loginUserData, packet);
	auto allData = PacketCreate::allBinaryPacket(loginUserData);
	ConnectionManager::instance()->sendBinaryMessage(user_id, allData);
}

