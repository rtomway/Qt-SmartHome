#ifndef NETWORKSERVICE_H_
#define NETWORKSERVICE_H_

#include <QObject>

#include "HttpClientPort.h"

class NetWorkService :public QObject
{
	Q_OBJECT
public:
	NetWorkService(HttpClientPort* httpClientPort, HttpClientPort* externalHttpClientPort,QObject* parent = nullptr);
	~NetWorkService();
public:
	using HttpCallback = std::function<void(const QJsonObject&, const QByteArray&)>;
	void sendHttpGetRequest(const QString& path, const QUrlQuery& params, const QMap<QString, QString>& headers, HttpCallback callback = nullptr);
	void sendHttpPostRequest(const QString& path, const QByteArray& data, const QMap<QString, QString>& headers, HttpCallback callback = nullptr);
	void sendHttpGetRequest(const QString& path, const QUrlQuery& params, HttpCallback callback = nullptr);
	void sendHttpPostRequest(const QString& path, const QByteArray& data, HttpCallback callback = nullptr);
	void sendExternalHttpGetRequest(const QString& path, const QUrlQuery& params, const QMap<QString, QString>& headers, HttpCallback callback = nullptr);
	void sendExternalHttpGetRequest(const QString& path, const QUrlQuery& params,HttpCallback callback = nullptr);
private:
	HttpClientPort* m_httpClientPort;
	HttpClientPort* m_externalHttpClientPort;
};


#endif // !NETWORKSERVICE_H_
