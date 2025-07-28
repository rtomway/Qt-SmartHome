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
	// 使用 std::function 定义回调类型
	using HttpCallback = std::function<void(const QJsonObject&, const QByteArray&)>;
	// 发送 HTTP 请求，接收回调函数
	void get(const QString& type, const QUrlQuery& params, const QMap<QString, QString>& headers = QMap<QString, QString>(), HttpCallback callback = nullptr)override;
	void post(const QString& type, const QByteArray& data, const QMap<QString, QString>& headers = QMap<QString, QString>(), HttpCallback callback = nullptr)override;
private:
	void setHeaders(QNetworkRequest& request, const QMap<QString, QString>& headers = QMap<QString, QString>());
	void handleReply(QNetworkReply* reply, HttpCallback callback);
	void replyErrorHandle(QNetworkReply::NetworkError error);
	void replyDataHandle(QNetworkReply* reply, HttpCallback callBack);
private:
	QNetworkAccessManager* m_networkManager{};
};

#endif // EXTERNALHTTPCLIENT_H
