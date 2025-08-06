#include "MqttClient.h"
#include <QJsonOBject>
#include <QJsonDocument>

#include <QSslSocket>
#include <QFile>

MqttClient::MqttClient(QObject* parent)
	: QObject(parent)
	, m_mqttClient(new QMqttClient(this))
{
	// 连接信号槽
	connect(m_mqttClient, &QMqttClient::stateChanged, this, &MqttClient::onStateChanged);
	connect(m_mqttClient, &QMqttClient::messageReceived, this, &MqttClient::onMessageReceived);
}

MqttClient::~MqttClient()
{
	disconnectFromBroker();
	delete m_mqttClient;
}

void MqttClient::init()
{
	this->connectToBroker("host", "smartHome-HostMachine", "200600xx");

	connect(m_mqttClient, &QMqttClient::connected, this, [=]
		{
			initSubscribe();
		});

}

//连接onenet
void MqttClient::connectToBroker(const QString& clientId, const QString& userName, const QString& password)
{

	// OneNET 的固定连接信息
	m_mqttClient->setHostname("m1b11b11.ala.cn-hangzhou.emqxsl.cn");
	m_mqttClient->setPort(8883);

	m_mqttClient->setClientId(clientId);
	m_mqttClient->setUsername(userName);
	m_mqttClient->setPassword(password);
	m_mqttClient->setProtocolVersion(QMqttClient::MQTT_5_0);

	m_mqttClient->setKeepAlive(60);
	m_mqttClient->setCleanSession(true);                   // 设置会话标志位true

	// 配置 SSL
	QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
	sslConfig.setProtocol(QSsl::TlsV1_2OrLater); // 强制 TLS 1.2+
	sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer); // 验证证书

	// 发起加密连接
	m_mqttClient->connectToHostEncrypted(sslConfig);
}

void MqttClient::disconnectFromBroker()
{
	if (m_mqttClient->state() != QMqttClient::Disconnected)
	{
		m_mqttClient->disconnectFromHost();
	}
}

bool MqttClient::isConnected() const
{
	return m_mqttClient->state() == QMqttClient::Connected;
}

QMqttClient::ClientState MqttClient::state() const
{
	return m_mqttClient->state();
}

void MqttClient::onStateChanged(QMqttClient::ClientState state)
{
	switch (state)
	{
	case QMqttClient::Connected:
		emit connected();
		break;
	case QMqttClient::Disconnected:
		emit disconnected();
		break;
	default:
		break;
	}
}

//订阅初始化
void MqttClient::initSubscribe()
{
	this->subscribe("smartHome/data",0);
}

//主题订阅
void MqttClient::subscribe(const QString& topic, quint8 qos)
{
	if (m_mqttClient->state() == QMqttClient::Connected)
	{
		auto subscription = m_mqttClient->subscribe(topic, qos);
		if (!subscription)
		{
			qWarning() << "Failed to subscribe to topic:" << topic;
		}
	}
	else
	{
		qWarning() << "Cannot subscribe - client not connected";
	}
}

//取消订阅
void MqttClient::unsubscribe(const QString& topic)
{
	if (m_mqttClient->state() == QMqttClient::Connected)
	{
		m_mqttClient->unsubscribe(topic);
	}
}

//信息发布
void MqttClient::publishCmd(const QString& topic, const QByteArray& message, quint8 qos, bool retain)
{
	if (m_mqttClient->state() == QMqttClient::Connected)
	{
		qint32 id = m_mqttClient->publish(topic, message, qos, retain);
		if (id < 0)
		{
			qWarning() << "Failed to publish message to topic:" << topic;
		}
		else
		{
			emit messagePublished(id);
		}
	}
	else 
	{
		qWarning() << "Cannot publish - client not connected";
	}
}


//模块数据接收
void MqttClient::moduleDataReceived(const QString& module, const QJsonObject& data)
{

}

//信息接收
void MqttClient::onMessageReceived(const QByteArray& message, const QMqttTopicName& topic)
{
	QJsonParseError err;
	QJsonDocument doc = QJsonDocument::fromJson(message, &err);
	if (err.error != QJsonParseError::NoError)
	{
		qWarning() << "JSON error:" << err.errorString();
		return;
	}

	QJsonObject obj = doc.object();
	qDebug() << "接收数据:" << obj;

	// 处理模块数据上报
	QString module = obj["module"].toString();
	QJsonObject data = obj["data"].toObject();
	if (!module.isEmpty())
	{
		emit moduleDataReceived(module, data);
	}
}