#include "LoginUserManager.h"
#include <QJsonDocument>

#include "ImageUtil.h"
#include "TokenManager.h"
#include "EventBus.h"
#include "../Client-ServiceLocator/NetWorkServiceLocator.h"
#include "myMessageBox/myMessageBox.h"


LoginUserManager* LoginUserManager::instance()
{
	static LoginUserManager instance;
	return &instance;
}

LoginUserManager::LoginUserManager()
{
	//保存token并连接登录
	connect(EventBus::instance(), &EventBus::loginValidationSuccess, this, &LoginUserManager::onLoginValidationSuccess);
}

//设置当前用户信息
void LoginUserManager::onInitLoginUser(const QJsonObject& loginUserObj)
{
	//加载当前登录用户信息
	m_loginUser_name = loginUserObj["user_name"].toString();

	//向内部发送信号
	emit loginUserLoadSuccess(loginUserObj);
}

//登录验证成功
void LoginUserManager::onLoginValidationSuccess(const QJsonObject& paramsObject)
{
	//当前用户id
	m_loginUser_id = paramsObject["user_id"].toString();
	//保存token
	auto token = paramsObject["token"].toString();
	TokenManager::saveToken(token);

	QJsonObject loginObj;
	loginObj["user_id"] = m_loginUser_id;
	QJsonDocument loginDoc(loginObj);
	auto data = loginDoc.toJson(QJsonDocument::Compact);

	NetWorkServiceLocator::instance()->sendHttpPostRequest("login", data,
		[=](const QJsonObject& obj, const QByteArray& data)
		{
			auto paramsObj = obj["params"].toObject();
			int code = obj["code"].toInt();
			if (code == 0)
			{
				auto user_id = paramsObj["user_id"].toString();
				//头像保存文件指定
				ImageUtils::setLoginUser(user_id);
				ImageUtils::saveAvatarToLocal(data, user_id, [=]()
					{
						this->onInitLoginUser(obj);
					});
			}
			else
			{
				MyMessageBox box(nullptr, "错误", obj["message"].toString());
			}
		});

}

//获取登录用户Id
const QString& LoginUserManager::getLoginUserId() const
{
	return m_loginUser_id;
}

//获取登录用户姓名
const QString& LoginUserManager::getLoginUserName() const
{
	return m_loginUser_name;
}

//清除信息
void LoginUserManager::clearLoginUserManager()
{
	m_loginUser_id = QString();
	m_loginUser_name = QString();
}
