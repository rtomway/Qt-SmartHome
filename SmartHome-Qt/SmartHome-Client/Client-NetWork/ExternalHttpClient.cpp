#include "ExternalHttpClient.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>

ExternalHttpClient::ExternalHttpClient(QObject* parent)
	: HttpClientPort(parent)
	, m_networkManager(new QNetworkAccessManager(this))
{

}

//外部http get请求
void ExternalHttpClient::get(const QString& path, const QUrlQuery& params, const QMap<QString, QString>& headers, HttpCallback callback)
{
	// 构造URL
	QUrl url(path);
	if (!params.isEmpty())
	{
		url.setQuery(params);
	}

	QNetworkRequest request(url);

	// 添加所有请求头
	this->setHeaders(request, headers);

	// 发送GET请求
	QNetworkReply* reply = m_networkManager->get(request);
	//响应回复
	connect(reply, &QNetworkReply::finished, [this, reply, callback]()
		{
			this->handleReply(reply, callback);
		});
}
void ExternalHttpClient::get(const QString& path, const QUrlQuery& params, HttpCallback callback)
{
	this->get(path, params, QMap<QString, QString>(), callback);
}

//外部http post请求
void ExternalHttpClient::post(const QString& path, const QByteArray& data, const QMap<QString, QString>& headers, HttpCallback callback)
{

}
void ExternalHttpClient::post(const QString& path, const QByteArray& data, HttpCallback callback)
{
}

//设置http请求头
void ExternalHttpClient::setHeaders(QNetworkRequest& request, const QMap<QString, QString>& headers)
{
	//添加请求头
	for (auto it = headers.constBegin(); it != headers.constEnd(); ++it)
	{
		request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
	}
	//默认application/json
	if (!headers.contains("Content-Type"))
	{
		request.setRawHeader("Content-Type", "application/json");
	}
}

//回复响应处理
void ExternalHttpClient::handleReply(QNetworkReply* reply, HttpCallback callback)
{
	// 错误处理
	if (reply->error() != QNetworkReply::NoError)
	{
		replyErrorHandle(reply->error());
		reply->deleteLater();
		return;
	}

	// 检查 HTTP 状态码
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	if (statusCode == 204)
	{
		// 无响应体，但请求成功
		reply->deleteLater();
		return;
	}

	//处理响应
	replyDataHandle(reply, callback);
	reply->deleteLater();
}

//错误处理
void ExternalHttpClient::replyErrorHandle(QNetworkReply::NetworkError error)
{
	switch (error)
	{
	case QNetworkReply::TimeoutError:
		qDebug() << "The request timed out.";
		break;
	case QNetworkReply::HostNotFoundError:
		qDebug() << "The host was not found.";
		break;
	case QNetworkReply::ConnectionRefusedError:
		qDebug() << "The connection was refused.";
		break;
		// 其他错误处理
	default:
		qDebug() << "Other error:";
	}
}

//处理回复数据
void ExternalHttpClient::replyDataHandle(QNetworkReply* reply, HttpCallback callBack)
{
	//处理响应
	QByteArray responseData = reply->readAll();
	QByteArray contentType = reply->rawHeader("Content-Type");
	//调用回调
	if (callBack)
	{
		//返回json信息
		if (contentType.contains("application/json"))
		{
			//回调,主线程进行，GuI操作
			QMetaObject::invokeMethod(QCoreApplication::instance(), [callBack, responseData]()
				{
					callBack(QJsonDocument::fromJson(responseData).object(), QByteArray());
				});
			return;
		}
	}
}
