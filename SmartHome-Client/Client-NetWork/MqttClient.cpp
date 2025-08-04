#include "MqttClient.h"
#include <QDebug>
#include <stdio.h>

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
	this->connectToBroker("R4EaNCH6HJ", "HostMachine", "version=2018-10-31&res=products%2FR4EaNCH6HJ%2Fdevices%2FHostMachine&et=1754653246&method=md5&sign=ZxZwGLQxQtDSb%2BR4qj9bmQ%3D%3D");
}

void MqttClient::connectToBroker(const QString& productId, const QString& deviceName, const QString& devicePassword)
{
	// OneNET 的固定连接信息
	m_mqttClient->setHostname("www.onenet.hk.chinamobile.com");
	m_mqttClient->setPort(1883);

	m_mqttClient->setClientId(deviceName);
	m_mqttClient->setUsername(productId);
	m_mqttClient->setPassword(devicePassword);
	m_mqttClient->setProtocolVersion(QMqttClient::MQTT_3_1_1);

	m_mqttClient->setKeepAlive(60);

	m_mqttClient->connectToHost();

	connect(m_mqttClient, &QMqttClient::connected, this, [=]
		{
			qDebug() << "连接成功";
			m_productId = productId;
			m_deviceName = deviceName;
		});

	
}

void MqttClient::disconnectFromBroker()
{
	if (m_mqttClient->state() != QMqttClient::Disconnected)
	{
		m_mqttClient->disconnectFromHost();
	}
}

void MqttClient::subscribe(const QString& topic, quint8 qos)
{
	if (m_mqttClient->state() == QMqttClient::Connected)
	{
		auto subscription = m_mqttClient->subscribe(topic, qos);
		if (!subscription) {
			qWarning() << "Failed to subscribe to topic:" << topic;
		}
	}
	else
	{
		qWarning() << "Cannot subscribe - client not connected";
	}
}

void MqttClient::unsubscribe(const QString& topic)
{
	if (m_mqttClient->state() == QMqttClient::Connected)
	{
		m_mqttClient->unsubscribe(topic);
	}
}

void MqttClient::publish(const QString& topic, const QByteArray& message,
	quint8 qos, bool retain)
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
	else {
		qWarning() << "Cannot publish - client not connected";
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

void MqttClient::onMessageReceived(const QByteArray& message, const QMqttTopicName& topic)
{
	emit messageReceived(topic.name(), message);
}