#include "MessageHandle.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QIODevice>
#include <QWebSocket>

#include "ImageUtil.h"
#include "CreateId.h"
#include "PacketCreate.h"
#include "DataBaseQuery.h"
#include "jwt-cpp/jwt.h"
#include "Server-MessageHandle/LoginHandle.h"
#include "Server-MessageHandle/RegisterHandle.h"
#include "Server-MessageHandle/UserHandle.h"

MessageHandle::MessageHandle(QObject* parent)
	:QObject(parent)
{
	initWebRequestHash();
	initHttpRequestHash();
	initPublicPageType();
}
//映射表
void MessageHandle::initWebRequestHash()
{
	webRequestHash["login"] = LoginHandle::handle_login;
}
void MessageHandle::initHttpRequestHash()
{
	httpRequestHash["/loginValidation"] = LoginHandle::handle_loginValidation;
	httpRequestHash["/updateUserAvatar"] = UserHandle::handle_updateUserAvatar;
}

//public操作
void MessageHandle::initPublicPageType()
{
	m_publicPage_list.append("/loginValidation");
}

//token验证
bool MessageHandle::webTokenVerify(const QString& token, const QString& user_id, const QString& type)
{
	if (m_publicPage_list.contains(type))
	{
		return true;
	}

	try {
		auto decoded = jwt::decode(token.toStdString());

		// 校验签名
		auto verifier = jwt::verify()
			.allow_algorithm(jwt::algorithm::hs256{ "xu-server" })
			.with_issuer("auth0");

		verifier.verify(decoded);

		// 检查 payload 中的 user_id
		std::string payload_user_id = decoded.get_payload_claim("user_id").as_string();
		if (payload_user_id != user_id.toStdString())
		{
			qDebug() << "[Token无效] user_id不匹配";
			return false;
		}

		return true;
	}
	catch (const std::exception& e)
	{
		qDebug() << "[Token验证失败]" << e.what();
		return false;
	}
}
bool MessageHandle::httpToeknVerify(const QString& type, const QHttpServerRequest& request)
{
	//公共界面访问无需验证
	if (m_publicPage_list.contains(type))
	{
		return true;
	}
	//获取需验证的信息
	QString token;
	QString user_id;
	for (auto& requestHeader : request.headers())
	{
		if (requestHeader.first == "user_id")
		{
			user_id = requestHeader.second;
		}
		if (requestHeader.first == "Authorization")
		{
			token = requestHeader.second;
		}
	}
	// 可能是 Bearer 开头，记得处理
	if (token.startsWith("Bearer "))
		token = token.mid(7);
	//验证
	try {
		auto decoded = jwt::decode(token.toStdString());

		// 校验签名
		auto verifier = jwt::verify()
			.allow_algorithm(jwt::algorithm::hs256{ "xu-server" })
			.with_issuer("auth0");

		verifier.verify(decoded);

		// 检查 payload 中的 user_id
		std::string payload_user_id = decoded.get_payload_claim("user_id").as_string();
		if (payload_user_id != user_id.toStdString())
		{
			qDebug() << "[Token无效] user_id不匹配";
			return false;
		}

		return true;
	}
	catch (const std::exception& e)
	{
		qDebug() << "[Token验证失败]" << e.what();
		return false;
	}
}

//web消息处理
void MessageHandle::webTextHandler(const QString& message, QWebSocket* socket)
{
	QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
	if (doc.isObject())
	{
		QJsonObject obj = doc.object();
		QString client_id;
		auto type = obj["type"].toString();
		auto paramsObject = obj["params"].toObject();
		client_id = paramsObject["user_id"].toString();
		if (!client_id.isEmpty())
		{
			auto token = paramsObject["token"].toString();
			if (!webTokenVerify(token, client_id, type))
			{
				qDebug() << "token认证失败";
				return;
			}
			emit this->addClient(client_id, socket);
		}
		qDebug() << "客户端发来消息:" << client_id << "type:" << type;
		//根据类型给处理函数处理
		if (webRequestHash.contains(type))
		{
			auto handle = webRequestHash[type];
			handle(paramsObject, QByteArray());
		}
		else
		{
			qDebug() << "未知的类型:" << type;
		}
	}
}
void MessageHandle::webBinaryHandler(const QByteArray& message, QWebSocket* socket)
{
	auto parsePacketList = PacketCreate::parseDataPackets(message);
	for (auto& parsePacket : parsePacketList)
	{
		qDebug() << "客户端发来消息type:" << parsePacket.type;
		//验证token
		auto token = parsePacket.params["token"].toString();
		auto user_id = parsePacket.params["user_id"].toString();
		if (!webTokenVerify(token, user_id, parsePacket.type))
		{
			qDebug() << "token认证失败";
			return;
		}
		// 根据类型给处理函数处理
		if (webRequestHash.contains(parsePacket.type)) {
			auto handle = webRequestHash[parsePacket.type];
			handle(parsePacket.params, parsePacket.data);  // 调用对应的处理函数
		}
		else {
			qDebug() << "未知的类型:" << parsePacket.type;
		}
	}
}

//http消息处理
void MessageHandle::httpGetHandler(const QHttpServerRequest& request, QHttpServerResponder& response)
{
	auto url = request.url();
	auto type = url.path();
	//token验证
	if (!httpToeknVerify(type, request))
	{
		qDebug() << "token验证失败";
		return;
	}
	//参数数据
	QUrlQuery query(url);
	QJsonObject paramsObj;
	for (const auto& item : query.queryItems())
	{
		paramsObj[item.first] = item.second;
	}

	// 根据类型给处理函数处理
	if (httpRequestHash.contains(type))
	{
		auto handle = httpRequestHash[type];
		handle(paramsObj, QByteArray(), response);  // 调用对应的处理函数
	}
	else
	{
		qDebug() << "未知的类型:" << type;
	}
}
void MessageHandle::httpPostTextHandler(const QHttpServerRequest& request, QHttpServerResponder& response)
{
	auto url = request.url();
	auto type = url.path();
	//token验证
	if (!httpToeknVerify(type, request))
	{
		qDebug() << "token验证失败";
		return;
	}
	//数据
	auto requestBody = request.body();
	auto paramsObj = QJsonDocument::fromJson(requestBody).object();
	// 根据类型给处理函数处理
	if (httpRequestHash.contains(type))
	{
		auto handle = httpRequestHash[type];
		handle(paramsObj, QByteArray(), response);  // 调用对应的处理函数
	}
	else
	{
		qDebug() << "未知的类型:" << type;
	}
}
void MessageHandle::httpPostBinaryHandler(const QHttpServerRequest& request, QHttpServerResponder& response)
{
	auto url = request.url();
	auto type = url.path();
	//token验证
	if (!httpToeknVerify(type, request))
	{
		qDebug() << "token验证失败";
		return;
	}
	auto message = request.body();
	auto parsePacketList = PacketCreate::parseDataPackets(message);
	for (auto& parsePacket : parsePacketList)
	{
		// 根据类型给处理函数处理
		if (httpRequestHash.contains(type))
		{
			auto handle = httpRequestHash[type];
			handle(parsePacket.params, parsePacket.data, response);  // 调用对应的处理函数
		}
		else
		{
			qDebug() << "未知的类型:" << type;
		}
	}
}




