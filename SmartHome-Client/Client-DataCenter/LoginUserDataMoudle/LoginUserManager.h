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
	//当前登录用户
	void onInitLoginUser(const QJsonObject& loginUserObj);
	const QString& get_loginUser_id()const;
	const QString& get_loginUser_name()const;
	//清除
	void clearLoginUserManager();
private:
	//当前登录用户
	QString m_loginUser_id;
	QString m_loginUser_name;
signals:
	void loginUserLoadSuccess(const QJsonObject& loginUserObj);
};


#endif // !LOGINUSERMANAGER_H_