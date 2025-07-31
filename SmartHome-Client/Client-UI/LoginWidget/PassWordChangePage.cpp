#include "PassWordChangePage.h"
#include "ui_PassWordChangePage.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

#include "../Client-ServiceLocator/NetWorkServiceLocator.h"
#include "EventBus.h"
#include "myMessageBox/myMessageBox.h"

PassWordChangePage::PassWordChangePage(QWidget* parent)
	:QWidget(parent)
	, ui(new Ui::PassWordChangePage)
{
	ui->setupUi(this);
	init();
}

PassWordChangePage::~PassWordChangePage()
{

}

void PassWordChangePage::init()
{
	initUi();

	//密码可视
	connect(ui->visableBtn, &QPushButton::clicked, this, &PassWordChangePage::onPasswordVisable);
	//取消返回
	connect(ui->cancelBtn, &QPushButton::clicked, this, &PassWordChangePage::hide);
	//修改密码
	connect(ui->okBtn, &QPushButton::clicked, this, &PassWordChangePage::onChangePassword);

}

//ui初始化
void PassWordChangePage::initUi()
{
	this->setFixedSize(700, 400);
	this->setObjectName("PassWordChangePage");
	this->setWindowIconText("密码修改");
	this->setAttribute(Qt::WA_StyledBackground, true);
	QFile file(":/stylesheet/Resource/StyleSheet/PasswordChangePage.css");
	if (file.open(QIODevice::ReadOnly))
	{
		this->setStyleSheet(file.readAll());
	}
	else
	{
		qDebug() << "stylesheet failed";
	}

	ui->accountEdit->setPlaceholderText("账号输入");
	ui->confidentialEdit->setPlaceholderText("密保输入");
	ui->passwordEdit->setPlaceholderText("输入新密码");
	ui->passwordEdit->setEchoMode(QLineEdit::Password);
	ui->okBtn->setText("确认");
	ui->cancelBtn->setText("取消");
	ui->accountEdit->setFixedWidth(200);
	ui->confidentialEdit->setFixedWidth(200);
	ui->passwordEdit->setFixedWidth(200);
}

//密码可视
void PassWordChangePage::onPasswordVisable()
{
	m_passwordIsVisable = !m_passwordIsVisable;
	if (m_passwordIsVisable)
	{
		ui->passwordEdit->setEchoMode(QLineEdit::Normal);
		ui->visableBtn->setStyleSheet(QString("border-image: url(:/icon/Resource/Icon/showPassword.png);"));
	}
	else
	{
		ui->passwordEdit->setEchoMode(QLineEdit::Password);
		ui->visableBtn->setStyleSheet(QString("border-image: url(:/icon/Resource/Icon/hidePassword.png);"));
	}
}

//修改密码
void PassWordChangePage::onChangePassword()
{
	
	if (ui->accountEdit->text().isEmpty())
	{
		MyMessageBox box(this, "警告", "账号不能为空",500);
		return;
	}
	if (ui->passwordEdit->text().isEmpty())
	{
		MyMessageBox box(this, "警告", "个人密保不能为空",500);
		return;
	}
	if (ui->passwordEdit->text().isEmpty())
	{
		MyMessageBox box(this, "警告", "密码不能为空",500);
		return;
	}

	//向服务器发送注册信息
	QJsonObject passwordChangeObj;
	passwordChangeObj["user_id"] = ui->accountEdit->text();
	passwordChangeObj["password"] = ui->passwordEdit->text();
	passwordChangeObj["confidential"] = ui->confidentialEdit->text();
	QJsonDocument doc(passwordChangeObj);
	auto data = doc.toJson(QJsonDocument::Compact);

	NetWorkServiceLocator::instance()->sendHttpPostRequest("passwordChange", data,
		[this](const QJsonObject& obj, const QByteArray& data)
		{
			auto changeObj = obj["params"].toObject();
			auto code= obj["code"].toInt();
			if (code==0)
			{
				MyMessageBox box(this, "成功", "密码修改成功",500);
				this->hide();
				EventBus::instance()->emit passwordChangeSuccess();
			}
			else
			{
				MyMessageBox box(this, "错误", obj["message"].toString(), 500);
			}

		});
}
