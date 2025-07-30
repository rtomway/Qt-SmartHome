#ifndef EXTERNALHTTPCLIENT_H
#define EXTERNALHTTPCLIENT_H

#include <QObject>
#include <QNetWorkAccessManager>
#include <QNetworkReply>

#include "HttpClientPort.h"

class ExternalHttpClient : public HttpClientPort
{
	Q_OBJECT
public:
	explicit ExternalHttpClient(QObject* parent = nullptr);
	using HttpCallback = std::function<void(const QJsonObject&, const QByteArray&)>;
	// 发送 HTTP 请求，接收回调函数
	void get(const QString& path, const QUrlQuery& params, const QMap<QString, QString>& headers , HttpCallback callback = nullptr)override;
	void post(const QString& path, const QByteArray& data, const QMap<QString, QString>& headers , HttpCallback callback = nullptr)override;
	void get(const QString& path, const QUrlQuery& params, HttpCallback callback = nullptr)override;
	void post(const QString& path, const QByteArray& data, HttpCallback callback = nullptr)override;
private:
	void setHeaders(QNetworkRequest& request, const QMap<QString, QString>& headers = QMap<QString, QString>());
	void handleReply(QNetworkReply* reply, HttpCallback callback);
	void replyErrorHandle(QNetworkReply::NetworkError error);
	void replyDataHandle(QNetworkReply* reply, HttpCallback callBack);
private:
	QNetworkAccessManager* m_networkManager{};
};

#endif // EXTERNALHTTPCLIENT_H
