#include "MessageHandle.h"
#include <QJsonDocument>
#include <QJsonArray>

#include "PacketCreate.h"
#include "SersorDataHandle.h"


MessageHandle::MessageHandle(QObject* parent)
	: QObject(parent)
{
	initRequestHash();
}


//注册表
void MessageHandle::initRequestHash()
{
	m_requestHash["temperatureSersor"] = SersorDataHandle::updateIndoorTemperature;
	m_requestHash["humiditySersor"] = SersorDataHandle::updateIndoorHumidity;
}

//文本消息处理
void MessageHandle::handle_textMessage(const QString& message)
{
	qDebug() << "ddddddd:" << message;
	auto messageDoc = QJsonDocument::fromJson(message.toUtf8());
	if (messageDoc.isObject())
	{
		auto sersorObj = messageDoc.object();
		auto product = sersorObj["product"].toString();
		qDebug() << "接收传感器数据:" << product;
		QByteArray data;
		if (m_requestHash.contains(product))
		{
			auto& handle = m_requestHash[product];
			handle(sersorObj, data);
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
		if (m_requestHash.contains(parsePacket.type))
		{
			auto& handle = m_requestHash[parsePacket.type];
			handle(parsePacket.params, parsePacket.data);
		}
		else
		{

		}
	}
}
