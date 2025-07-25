#include "LoginWidget.h"
#include "ui_LoginWidget.h"
#include <QFile>
#include <QMessageBox>
#include <QJsonDocument>

#include "ImageUtil.h"
#include "SConfigFile.h"
#include "RegisterPage.h"
#include "../Client-ServiceLocator/NetWorkServiceLocator.h"
#include "EventBus.h"
#include "LoginUserManager.h"


LoginWidget::LoginWidget(QWidget* parent)
	: AngleRoundedWidget(parent)
	, ui(new Ui::LoginWidget)
{
	ui->setupUi(this);
	init();
	QFile file(":/stylesheet/Resource/StyleSheet/LoginWidget.css");
	if (file.open(QIODevice::ReadOnly))
	{
		setStyleSheet(file.readAll());
	}
	else
	{
		qDebug() << file.fileName() << "打开失败";
	}
}

void LoginWidget::init()
{
	this->setObjectName("login");
	this->setWindowFlag(Qt::FramelessWindowHint);
	setFixedSize(720, 480);

	ui->accountLab->setText("账户:");
	ui->passwordLab->setText("密码:");
	ui->loginBtn->setText("登录");
	ui->registerBtn->setText("注册");
	ui->rememberLab->setText("记住密码");
	ui->forgetPasBtn->setText("忘记密码");
	ui->exitBtn->setIcon(QIcon(":/icon/Resource/Icon/x.png"));

	ui->passwordEdit->setEchoMode(QLineEdit::Password);
	ui->remenberCheck->setChecked(true);

	//读取登录信息配置
	SConfigFile config("config.ini");
	QFile configFile("config.ini");
	ui->accountEdit->setText(config.value("user_id").toString());
	ui->passwordEdit->setText(config.value("password").toString());

	//登录
	connect(ui->loginBtn, &QPushButton::clicked, this, &LoginWidget::onLoginRequest);
	//注册
	connect(ui->registerBtn, &QPushButton::clicked, this, &LoginWidget::onRegister);
	//忘记密码
	connect(ui->forgetPasBtn, &QPushButton::clicked, this, &LoginWidget::onForgetPassword);
	//退出界面
	connect(ui->exitBtn, &QPushButton::clicked, this, &LoginWidget::hide);
	//登录成功
	connect(LoginUserManager::instance(), &LoginUserManager::loginUserLoadSuccess, this, &LoginWidget::onLoginSuccess);
}

//发送登录请求
void LoginWidget::onLoginRequest()
{
	auto user_id = ui->accountEdit->text();
	auto password = ui->passwordEdit->text();
	if (user_id.isEmpty() || password.isEmpty())
	{
		QMessageBox::warning(nullptr, "警告", "账号或密码不能为空");
		return;
	}

	//装载信息
	QJsonObject loginObj;
	loginObj["user_id"] = user_id;
	loginObj["password"] = password;
	QJsonDocument doc(loginObj);
	auto data = doc.toJson(QJsonDocument::Compact);

	//发送
	NetWorkServiceLocator::instance()->sendHttpRequest("loginValidation", data, "application/json");

}

//注册
void LoginWidget::onRegister()
{

}

//忘记密码
void LoginWidget::onForgetPassword()
{

}

//成功登录
void LoginWidget::onLoginSuccess()
{
	//切换界面
	emit Loginsuccess();
	//记录个人信息
	auto user_id = ui->accountEdit->text();
	auto password = ui->passwordEdit->text();
	SConfigFile config("config.ini");
	config.setValue("user_id", user_id);
	if (ui->remenberCheck->isChecked())
	{
		config.setValue("password", password);
	}
	else
	{
		config.removeValue("password");
	}
}

