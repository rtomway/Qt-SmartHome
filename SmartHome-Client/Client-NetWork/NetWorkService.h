﻿#ifndef NETWORKSERVICE_H_
#define NETWORKSERVICE_H_

#include <QObject>

#include "WebClientPort.h"
#include "HttpClientPort.h"
#include "MessageHandle.h"

class NetWorkService :public QObject
{
	Q_OBJECT
public:
	NetWorkService(WebClientPort* webClientPort, HttpClientPort* httpClientPort, QObject* parent = nullptr);
	~NetWorkService();
public:
	//连接建立
	void connectToServer(std::function<void()>callBack = nullptr);
	void disConnect();
	//消息发送
	void sendWebTextMessage(const QString& message);
	void sendWebBinaryData(const QByteArray& data);
	using HttpCallback = std::function<void(const QJsonObject&, const QByteArray&)>;
	void sendHttpGetRequest(const QString& path, const QUrlQuery& params, const QMap<QString, QString>& headers = QMap<QString, QString>(), HttpCallback callback = nullptr);
	void sendHttpPostRequest(const QString& path, const QByteArray& data, const QMap<QString, QString>& headers = QMap<QString, QString>(), HttpCallback callback = nullptr);
private:
	void init();
private:
	WebClientPort* m_webClientPort;
	HttpClientPort* m_httpClientPort;
	MessageHandle* m_messageHandle;
};


#endif // !NETWORKSERVICE_H_
