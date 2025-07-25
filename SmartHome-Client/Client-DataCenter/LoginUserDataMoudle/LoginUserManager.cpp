#include "LoginUserManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>

#include "ImageUtil.h"
#include "TokenManager.h"
#include "PacketCreate.h"
#include "EventBus.h"
#include "../Client-ServiceLocator/NetWorkServiceLocator.h"


LoginUserManager* LoginUserManager::instance()
{
	static LoginUserManager instance;
	return &instance;
}

LoginUserManager::LoginUserManager()
{
	//保存token并连接登录
	connect(EventBus::instance(), &EventBus::loginValidationSuccess, this, [=](const QJsonObject& paramsObject)
		{
			auto user_id = paramsObject["user_id"].toString();
			//保存token
			auto token = paramsObject["token"].toString();
			TokenManager::saveToken(token);

			//连接登录
			NetWorkServiceLocator::instance()->connectToServer([=]
				{
					QJsonObject connectObj;
					connectObj["user_id"] = user_id;
					auto message = PacketCreate::textPacket("login", connectObj);
					NetWorkServiceLocator::instance()->sendWebTextMessage(message);
				});
		});
	//登录用户初始化
	connect(EventBus::instance(), &EventBus::loginUserInit, this, &LoginUserManager::onInitLoginUser);
}

//设置当前用户信息
void LoginUserManager::onInitLoginUser(const QJsonObject& loginUserObj)
{
	//加载当前登录用户信息
	m_loginUser_id = loginUserObj["user_id"].toString();
	m_loginUser_name = loginUserObj["user_name"].toString();

	//向内部发送信号
	emit loginUserLoadSuccess(loginUserObj);
}

//获取登录用户Id
const QString& LoginUserManager::get_loginUser_id() const
{
	return m_loginUser_id;
}

//获取登录用户姓名
const QString& LoginUserManager::get_loginUser_name() const
{
	return m_loginUser_name;
}

//清除信息
void LoginUserManager::clearLoginUserManager()
{
	m_loginUser_id = QString();
	m_loginUser_name = QString();
}
