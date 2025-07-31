#ifndef	LOGINUSERMANAGER_H_
#define LOGINUSERMANAGER_H_

#include <QJsonObject>
#include <QObject>

class LoginUserManager : public QObject
{
	Q_OBJECT
public:
	static LoginUserManager* instance();
private:
	LoginUserManager();
	LoginUserManager(const LoginUserManager&) = delete;
	LoginUserManager& operator=(const LoginUserManager&) = delete;
public:
	const QString& getLoginUserId()const;
	const QString& getLoginUserName()const;
	//清除
	void clearLoginUserManager();
private:
	void onInitLoginUser(const QJsonObject& loginUserObj);
	void onLoginValidationSuccess(const QJsonObject& loginUserObj);
private:
	//当前登录用户
	QString m_loginUser_id;
	QString m_loginUser_name;
signals:
	void loginUserLoadSuccess(const QJsonObject& loginUserObj);
};


#endif // !LOGINUSERMANAGER_H_