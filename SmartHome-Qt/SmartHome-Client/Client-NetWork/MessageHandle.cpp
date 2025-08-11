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
	m_requestHash["temperatureSensor"] = SersorDataHandle::updateIndoorTemperature;
	m_requestHash["lightSensor"] = SersorDataHandle::updateIndoorHumidity;
}

//文本消息处理
void MessageHandle::handle_textMessage(const QString& message)
{
    qDebug() << "接收到原始消息:" << message;
    auto messageDoc = QJsonDocument::fromJson(message.toUtf8());
    if (!messageDoc.isObject()) 
    {
        qDebug() << "消息不是JSON对象";
        return;
    }

    QJsonObject rootObj = messageDoc.object();
    for (auto it = rootObj.begin(); it != rootObj.end(); ++it) 
    {
        QString sensorType = it.key(); 
        QJsonObject sensorData = it.value().toObject(); 

        qDebug() << "解析传感器类型:" << sensorType;
        if (m_requestHash.contains(sensorType)) 
        {
         
            auto& handleFunc = m_requestHash[sensorType];
            handleFunc(sensorData,QByteArray()); 
        }
        else 
        {
            qDebug() << "未注册的传感器类型:" << sensorType;
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
