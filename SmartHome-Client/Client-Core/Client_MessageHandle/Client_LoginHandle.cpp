#include "Client_LoginHandle.h"
#include <QPixmap>
#include <QJsonArray>
#include <QJsonDocument>

#include "../Client-ServiceLocator/NetWorkServiceLocator.h"
#include "EventBus.h"
#include "ImageUtil.h"
#include "LoginUserManager.h"

//登录认证成功
void Client_LoginHandle::handle_loginValidationSuccess(const QJsonObject& paramsObject, const QByteArray& data)
{
	EventBus::instance()->emit loginValidationSuccess(paramsObject);
}

//登录成功
void Client_LoginHandle::handle_loginSuccess(const QJsonObject& paramsObject, const QByteArray& data)
{
	QJsonObject loginUser = paramsObject;
	auto user_id = loginUser["user_id"].toString();
	ImageUtils::saveAvatarToLocal(data, user_id);
		
	EventBus::instance()->emit loginUserInit(loginUser);
}


//注册成功
void Client_LoginHandle::handle_registerSuccess(const QJsonObject& paramsObject, const QByteArray& data)
{
	EventBus::instance()->emit registerSuccess(paramsObject);
}
