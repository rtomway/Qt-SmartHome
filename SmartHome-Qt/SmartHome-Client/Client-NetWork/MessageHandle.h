#ifndef MESSAGEHANDLE_H_
#define MESSAGEHANDLE_H_

#include <QByteArray>
#include <QJsonObject>
#include <QObject>

class MessageHandle :public QObject
{
	Q_OBJECT
public:
	MessageHandle(QObject* parent = nullptr);
	//消息处理接口
	void handle_textMessage(const QString& message);
	void handle_binaryData(const QByteArray& data);
private:
	//消息处理函数表
	QHash<QString, std::function<void(const QJsonObject&, const QByteArray&)>> m_requestHash{};
private:
	void initRequestHash();
};

#endif // !MESSAGEHANDLE_H_
