#include "NetWorkService.h"

NetWorkService::NetWorkService(WebClientPort* webClientPort, HttpClientPort* httpClientPort, HttpClientPort* externalHttpClientPort,QObject* parent)
	: m_webClientPort(webClientPort)
	, m_httpClientPort(httpClientPort)
	, m_externalHttpClientPort(externalHttpClientPort)
	, m_messageHandle(new MessageHandle(this))
{
	init();
}

NetWorkService::~NetWorkService()
{

}

//连接web服务器
void NetWorkService::connectToServer(std::function<void()>callBack)
{
	m_webClientPort->connectToServer("ws://localhost:8888", [callBack]
		{
			if (callBack)
				callBack();
		});
}

//断开连接
void NetWorkService::disConnect()
{
	m_webClientPort->disconnect();
}

//web发送
void NetWorkService::sendWebTextMessage(const QString& message)
{
	m_webClientPort->sendTextMessage(message);
}
void NetWorkService::sendWebBinaryData(const QByteArray& data)
{
	m_webClientPort->sendBinaryData(data);
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
	m_externalHttpClientPort->get(path, params,callback);
}

void NetWorkService::init()
{
	//接收消息
	connect(m_webClientPort, &WebClientPort::textMessage, m_messageHandle, &MessageHandle::handle_textMessage);
	connect(m_webClientPort, &WebClientPort::binaryData, m_messageHandle, &MessageHandle::handle_binaryData);
	connect(m_httpClientPort, &HttpClientPort::httpTextResponse, m_messageHandle, &MessageHandle::handle_textMessage);
	connect(m_httpClientPort, &HttpClientPort::httpDataResponse, m_messageHandle, &MessageHandle::handle_binaryData);
}
