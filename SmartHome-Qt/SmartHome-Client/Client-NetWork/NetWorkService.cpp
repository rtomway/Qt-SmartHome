#include "NetWorkService.h"
#include <QJsonDocument>


NetWorkService::NetWorkService(MqttClient* mqttClient, HttpClientPort* httpClientPort, HttpClientPort* externalHttpClientPort, QObject* parent)
	: QObject(parent)
	, m_mqttClient(mqttClient)
	, m_httpClientPort(httpClientPort)
	, m_externalHttpClientPort(externalHttpClientPort)
{

}

NetWorkService::~NetWorkService()
{

}

//http内部请求
void NetWorkService::sendHttpGetRequest(const QString& path, const QUrlQuery& params, const QMap<QString, QString>& headers, HttpCallback callback)
{
	m_httpClientPort->get(path, params, headers, callback);
}
void NetWorkService::sendHttpPostRequest(const QString& path, const QByteArray& data, const QMap<QString, QString>& headers, HttpCallback callback)
{
	m_httpClientPort->post(path, data, headers, callback);
}

void NetWorkService::sendHttpGetRequest(const QString& path, const QUrlQuery& params, HttpCallback callback)
{
	m_httpClientPort->get(path, params, callback);
}
void NetWorkService::sendHttpPostRequest(const QString& path, const QByteArray& data, HttpCallback callback)
{
	m_httpClientPort->post(path, data, callback);
}

//http外部请求
void NetWorkService::sendExternalHttpGetRequest(const QString& path, const QUrlQuery& params, const QMap<QString, QString>& headers, HttpCallback callback)
{
	m_externalHttpClientPort->get(path, params, headers, callback);
}
void NetWorkService::sendExternalHttpGetRequest(const QString& path, const QUrlQuery& params, HttpCallback callback)
{
	m_externalHttpClientPort->get(path, params, callback);
}

//mqtt连接onenet
void NetWorkService::connectToMqttBroker()
{
	m_mqttClient->init();
}

//mqtt断开onenet的连接
void NetWorkService::disConnectWithMqttBroker()
{
	m_mqttClient->disconnected();
}

//发布消息
void NetWorkService::publishCmd(const QString& topic, const QJsonObject& obj, quint8 qos, bool retain)
{
	QJsonDocument doc(obj);
	auto message = doc.toJson(QJsonDocument::Compact);
	m_mqttClient->publishCmd(topic, message, qos, retain);
}