﻿#include "Client_UserHandle.h"
#include <QPixmap>
#include <EventBus.h>
#include <QCoreApplication>


void Client_UserHandle::handle_updateUserAvatar(const QJsonObject& paramsObject, const QByteArray& data)
{
	// 将操作抛到主线程执行
	QMetaObject::invokeMethod(QCoreApplication::instance(), [paramsObject, data]() 
		{
			auto user_id = paramsObject["user_id"].toString();
			QPixmap avatar;
			if (!avatar.loadFromData(data))
			{
				qWarning() << "Failed to load avatar for user:" << user_id;
				return;
			}
		EventBus::instance()->emit updateUserAvatar(user_id, avatar);
		});
}
