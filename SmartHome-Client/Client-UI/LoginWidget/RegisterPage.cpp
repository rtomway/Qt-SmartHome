#include "RegisterPage.h"
#include "ui_RegisterPage.h"
#include <QFile>
#include <QMouseEvent>
#include <QJsonDocument>

#include "../Client-ServiceLocator/NetWorkServiceLocator.h"
#include "EventBus.h"
#include "myMessageBox/myMessageBox.h"

RegisterPage::RegisterPage(QWidget* parent)
	:QWidget(parent)
	, ui(new Ui::RegisterPage)
{
	ui->setupUi(this);
	init();

}

RegisterPage::~RegisterPage()
{
	delete ui;
}

void RegisterPage::init()
{
	initUi();
	//注册
	connect(ui->registerBtn, &QPushButton::clicked, this, &RegisterPage::onRegisterUser);
	connect(EventBus::instance(), &EventBus::registerSuccess, this, &RegisterPage::hide);
}

//ui
void RegisterPage::initUi()
{
	this->setObjectName("RegisterPage");
	this->installEventFilter(this);
	this->setFocus();
	this->setFixedSize(680, 460);
	this->setAttribute(Qt::WA_StyledBackground, true);
	QFile file(":/stylesheet/Resource/StyleSheet/RegisterPage.css");
	if (file.open(QIODevice::ReadOnly))
	{
		this->setStyleSheet(file.readAll());
	}
	else
	{
		qDebug() << file.fileName() << "打开失败";
	}

	ui->passwordEdit->installEventFilter(this);

	ui->titleLab->setObjectName("title");
	ui->headerLab->setObjectName("header");

	ui->nickNameEdit->setPlaceholderText("输入昵称");
	ui->passwordEdit->setPlaceholderText("输入密码");
	ui->confidential->setPlaceholderText("输入个人密保");

	ui->nickNameEdit->setAlignment(Qt::AlignCenter);
	ui->passwordEdit->setAlignment(Qt::AlignCenter);
	ui->passwordLab->setVisible(false);
}

//用户注册
void RegisterPage::onRegisterUser()
{
	if (ui->nickNameEdit->text().isEmpty())
	{
		MyMessageBox box(this, "警告", "昵称不能为空", 500);
		return;
	}
	if (ui->passwordEdit->text().isEmpty())
	{
		MyMessageBox box(this, "警告", "密码不能为空", 500);
		return;
	}
	//向服务器发送注册信息
	QJsonObject registerObj;
	registerObj["user_name"] = ui->nickNameEdit->text();
	registerObj["password"] = ui->passwordEdit->text();
	registerObj["confidential"] = ui->confidential->text();
	QJsonDocument doc(registerObj);
	auto data = doc.toJson(QJsonDocument::Compact);

	NetWorkServiceLocator::instance()->sendHttpPostRequest("register", data,
		[this](const QJsonObject& obj, const QByteArray& data)
		{
			if (obj["code"].toInt() == 0)
			{
				MyMessageBox box(this, "成功", "注册成功", 500);
				auto accountObj = obj["params"].toObject();
				EventBus::instance()->emit registerSuccess(accountObj);
				this->hide();
			}
			else
			{
				MyMessageBox box(this, "错误", "操作失败请稍后尝试", 500);
			}

		});
}

//控件事件重写
bool RegisterPage::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == ui->passwordEdit && event->type() == QEvent::FocusIn)
	{
		ui->passwordLab->setVisible(true);
	}
	else if (watched == ui->passwordEdit && event->type() == QEvent::FocusOut)
	{
		ui->passwordLab->setVisible(false);
	}
	//点击控件外地方时清除控件焦点
	if (watched == this && event->type() == QEvent::MouseButtonPress)
	{
		this->setFocus();
	}
	return false;
}
