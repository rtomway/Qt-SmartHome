#ifndef MESSAGEHANDLE_H_
#define MESSAGEHANDLE_H_

#include <QByteArray>
#include <QJsonObject>
#include <QObject>
#include <QHttpServer>

class MessageHandle :public QObject
{
	Q_OBJECT
public:
	MessageHandle(QObject* parent = nullptr);

	void httpGetHandler(const QHttpServerRequest& request, QHttpServerResponder& response);
	void httpPostBinaryHandler(const QHttpServerRequest& request, QHttpServerResponder& response);
	void httpPostTextHandler(const QHttpServerRequest& request, QHttpServerResponder& response);
private:
	//消息处理函数表
	QHash<QString, std::function<void(const QJsonObject&, const QByteArray&, QHttpServerResponder&)>>httpRequestHash;
	//公开界面操作列表
	QStringList m_publicPage_list;
private:
	void initHttpRequestHash();
	void initPublicPageType();
	bool httpToeknVerify(const QString& type, const QHttpServerRequest& request);

};

#endif // !MESSAGEHANDLE_H_
