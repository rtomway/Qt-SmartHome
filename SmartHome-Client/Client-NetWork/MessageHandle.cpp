#include "MessageHandle.h"
#include <QJsonDocument>
#include <QJsonArray>

#include "PacketCreate.h"

#include "Client_MessageHandle/Client_LoginHandle.h"

MessageHandle::MessageHandle(QObject* parent)
	: QObject(parent)
{
	initRequestHash();
}

//注册表
void MessageHandle::initRequestHash()
{
	requestHash["loginValidationSuccess"] = Client_LoginHandle::handle_loginValidationSuccess;
	requestHash["loginSuccess"] = Client_LoginHandle::handle_loginSuccess;
}

//文本消息处理
void MessageHandle::handle_textMessage(const QString& message)
{
	auto messageDoc = QJsonDocument::fromJson(message.toUtf8());
	if (messageDoc.isObject())
	{
		QJsonObject obj = messageDoc.object();
		auto type = obj["type"].toString();
		auto paramsObject = obj["params"].toObject();
		QByteArray data;
		if (requestHash.contains(type))
		{
			auto& handle = requestHash[type];
			handle(paramsObject, data);
		}
	}
}

//二进制数据处理
void MessageHandle::handle_binaryData(const QByteArray& data)
{
	auto parsePacketList = PacketCreate::parseDataPackets(data);
	for (auto& parsePacket : parsePacketList)
	{
		// 根据类型给处理函数处理
		if (requestHash.contains(parsePacket.type))
		{
			auto& handle = requestHash[parsePacket.type];
			handle(parsePacket.params, parsePacket.data);
		}
		else
		{
			
		}
	}
}
