#ifndef	LOGINUSERMANAGER_H_
#define LOGINUSERMANAGER_H_

#include <QJsonObject>
#include <QObject>
#include <QSharedPointer>

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
	//当前登录用户
	void initLoginUser(const QJsonObject& loginUserObj);
	const QString& getLoginUserID()const;
	const QString& getLoginUserName()const;
	//清除
	void clearLoginUserManager();
private:
	//当前登录用户
	QString m_loginUserId{};
	QString m_loginUserName{};
signals:
	void loginUserLoadSuccess();
};


#endif // !LOGINUSERMANAGER_H_