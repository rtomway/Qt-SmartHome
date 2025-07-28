﻿#ifndef MESSAGEHANDLE_H_
#define MESSAGEHANDLE_H_

#include <QByteArray>
#include <QJsonObject>
#include <QObject>
#include <QWebSocket>
#include <QHttpServer>

class MessageHandle :public QObject
{
	Q_OBJECT
public:
	MessageHandle(QObject* parent = nullptr);
	//消息处理接口
	void webTextHandler(const QString& message, QWebSocket* socket);
	void webBinaryHandler(const QByteArray& message, QWebSocket* socket);

	void httpGetHandler(const QHttpServerRequest& request, QHttpServerResponder& response);
	void httpPostBinaryHandler(const QHttpServerRequest& request, QHttpServerResponder& response);
	void httpPostTextHandler(const QHttpServerRequest& request, QHttpServerResponder& response);
private:
	//消息处理函数表
	QHash<QString, std::function<void(const QJsonObject&, const QByteArray&)>>webRequestHash;
	QHash<QString, std::function<void(const QJsonObject&, const QByteArray&, QHttpServerResponder&)>>httpRequestHash;
	//公开界面操作列表
	QStringList m_publicPage_list;
private:
	void initWebRequestHash();
	void initHttpRequestHash();
	void initPublicPageType();
	bool webTokenVerify(const QString& token, const QString& user_id, const QString& type);
	bool httpToeknVerify(const QString& type, const QHttpServerRequest& request);
signals:
	void addClient(const QString& user_id, QWebSocket* client);
};

#endif // !MESSAGEHANDLE_H_
