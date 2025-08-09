#ifndef NETWORKSERVICE_H_
#define NETWORKSERVICE_H_

#include <QObject>

#include "HttpClientPort.h"
#include "MqttClient.h"

class NetWorkService :public QObject
{
	Q_OBJECT
public:
	NetWorkService(MqttClient* mqttClient, HttpClientPort* httpClientPort, HttpClientPort* externalHttpClientPort, QObject* parent = nullptr);
	~NetWorkService();
public:
	using HttpCallback = std::function<void(const QJsonObject&, const QByteArray&)>;
	void sendHttpGetRequest(const QString& path, const QUrlQuery& params, const QMap<QString, QString>& headers, HttpCallback callback = nullptr);
	void sendHttpPostRequest(const QString& path, const QByteArray& data, const QMap<QString, QString>& headers, HttpCallback callback = nullptr);
	void sendHttpGetRequest(const QString& path, const QUrlQuery& params, HttpCallback callback = nullptr);
	void sendHttpPostRequest(const QString& path, const QByteArray& data, HttpCallback callback = nullptr);
	void sendExternalHttpGetRequest(const QString& path, const QUrlQuery& params, const QMap<QString, QString>& headers, HttpCallback callback = nullptr);
	void sendExternalHttpGetRequest(const QString& path, const QUrlQuery& params, HttpCallback callback = nullptr);

	void connectToMqttBroker();
	void disConnectWithMqttBroker();

	void publishCmd(const QString& topic, const QJsonObject& obj, quint8 qos = 1, bool retain = false);
private:
	HttpClientPort* m_httpClientPort;
	HttpClientPort* m_externalHttpClientPort;
	MqttClient* m_mqttClient;
};


#endif // !NETWORKSERVICE_H_
