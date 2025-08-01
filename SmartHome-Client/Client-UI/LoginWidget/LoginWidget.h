#ifndef LOGINWIDGET_H_
#define LOGINWIDGET_H_

#include <QWidget>

#include "AngleRoundedWidget.h"
#include "RegisterPage.h"
#include "PassWordChangePage.h"

#include "MaskBackgroundWidget/MaskBackgroundWidget.h"


namespace Ui {class LoginWidget; }

class LoginWidget :public MaskBackgroundWidget
{
	Q_OBJECT 
public:
	LoginWidget( QWidget* parent = nullptr);
private:
	void init();
	void initUi();
private:
	void onLoginRequest();
	void onRegister();
	void onForgetPassword();
	void onLoginSuccess();
	void onRegisterSuccess(const QJsonObject&obj);
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
