﻿#include "NetWorkService.h"


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

void NetWorkService::connectToServer(std::function<void()>callBack)
{
	m_webClientPort->connectToServer("ws://localhost:8888", [callBack]
		{
			if (callBack)
				callBack();
		});
}

void NetWorkService::disConnect()
{
	m_webClientPort->disconnect();
}

void NetWorkService::sendWebTextMessage(const QString& message)
{
	m_webClientPort->sendTextMessage(message);
}

void NetWorkService::sendWebBinaryData(const QByteArray& data)
{
	m_webClientPort->sendBinaryData(data);
}


void NetWorkService::sendHttpGetRequest(const QString& path, const QUrlQuery& params, const QMap<QString, QString>& headers, HttpCallback callback)
{
	m_httpClientPort->get(path, params, headers, callback);
}

void NetWorkService::sendHttpPostRequest(const QString& path, const QByteArray& data, const QMap<QString, QString>& headers, HttpCallback callback)
{
	m_httpClientPort->post(path, data, headers, callback);
}

void NetWorkService::sendExternalHttpGetRequest(const QString& path, const QUrlQuery& params, const QMap<QString, QString>& headers, HttpCallback callback)
{
	m_externalHttpClientPort->get(path, params, headers, callback);
}


void NetWorkService::init()
{
	//接收消息
	connect(m_webClientPort, &WebClientPort::textMessage, m_messageHandle, &MessageHandle::handle_textMessage);
	connect(m_webClientPort, &WebClientPort::binaryData, m_messageHandle, &MessageHandle::handle_binaryData);
	connect(m_httpClientPort, &HttpClientPort::httpTextResponse, m_messageHandle, &MessageHandle::handle_textMessage);
	connect(m_httpClientPort, &HttpClientPort::httpDataResponse, m_messageHandle, &MessageHandle::handle_binaryData);
}
