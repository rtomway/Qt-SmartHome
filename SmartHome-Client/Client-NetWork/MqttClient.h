﻿#ifndef MQTTCLIENT_H_
#define MQTTCLIENT_H_

#include <QMqttClient>
#include <QObject>

class MqttClient : public QObject
{
	Q_OBJECT

public:
	explicit MqttClient(QObject* parent = nullptr);
	~MqttClient();

	// 连接/断开连接
	void init();
	void connectToBroker(const QString& productId, const QString& deviceName, const QString& devicePassword);
	void disconnectFromBroker();

	// 订阅/取消订阅
	void subscribe(const QString& topic, quint8 qos = 0);
	void unsubscribe(const QString& topic);

	// 发布消息
	void publish(const QString& topic, const QByteArray& message,
		quint8 qos = 0, bool retain = false);

	// 状态查询
	bool isConnected() const;
	QMqttClient::ClientState state() const;

signals:
	// 连接状态信号
	void connected();
	void disconnected();
	void errorOccurred(QMqttClient::ClientError error);

	// 消息相关信号
	void messageReceived(const QString& topic, const QByteArray& message);
	void messagePublished(qint32 id);

private slots:
	void onStateChanged(QMqttClient::ClientState state);
	void onMessageReceived(const QByteArray& message, const QMqttTopicName& topic);

private:
	QMqttClient* m_mqttClient;
	QString m_productId;
	QString m_deviceName;
};

#endif // MQTTCLIENT_H_