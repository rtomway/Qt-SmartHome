﻿#ifndef EVENTBUS_H_
#define EVENTBUS_H_

#include <QObject>


class EventBus :public QObject
{
	Q_OBJECT
public:
	static EventBus* instance();
	EventBus(const EventBus&) = delete;
	EventBus& operator=(const EventBus&) = delete;
private:
	EventBus() {};
signals:   //接受到消息发送信号通知UI界面更新同步
	//登录
	void loginValidationSuccess(const QJsonObject& paramsObject);
	void loginUserInit(const QJsonObject& paramsObject);
	void registerSuccess(const QJsonObject& obj);
	void passwordChangeSuccess();
	void updateUserMessage(const QJsonObject& paramsObject);
	void updateUserAvatar(const QString& user_id, const QPixmap& pixmap);

};

#endif // !EVENTBUS_H_
