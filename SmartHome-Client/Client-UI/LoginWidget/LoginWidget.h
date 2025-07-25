#ifndef LOGINWIDGET_H_
#define LOGINWIDGET_H_

#include <QWidget>

#include "AngleRoundedWidget.h"
#include "RegisterPage.h"
#include "PassWordChangePage.h"


namespace Ui {class LoginWidget; }

class LoginWidget :public AngleRoundedWidget
{
	Q_OBJECT 
public:
	LoginWidget( QWidget* parent = nullptr);
private:
	void init();
private:
	void onLoginRequest();
	void onRegister();
	void onForgetPassword();
	void onLoginSuccess();

private:
	Ui::LoginWidget* ui;
signals:
	void editfinish();
	void Loginsuccess();
private:
	std::unique_ptr<RegisterPage> m_registerPage{};
	std::unique_ptr<PassWordChangePage> m_passwordChangePage{};
};





#endif // !LOGINWIDGET_H_
