#include "MainWidget.h"
#include "ui_MainWidget.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>

#include "ImageUtil.h"
#include "../Client-ServiceLocator/NetWorkServiceLocator.h"
#include "LoginUserManager.h"
#include "PacketCreate.h"


MainWidget::MainWidget(QWidget* parent)
	:AngleRoundedWidget(parent)
	, ui(new Ui::MainWidget)
	, m_pageBtnGroup(new QButtonGroup(this))
	, m_hoomPage(new HoomPage(this))
	, m_devicePage(new DevicePage(this))
	, m_roomPage(new RoomPage(this))
{
	ui->setupUi(this);
	init();
	
	QFile file(":/stylesheet/Resource/StyleSheet/MainWidget.css");
	if (file.open(QIODevice::ReadOnly))
	{
		setStyleSheet(file.readAll());
	}
	else {
		qDebug() << "样式表打开失败";
	}
}
MainWidget::~MainWidget()
{
	delete ui;
}

//界面初始化
void MainWidget::init()
{
	initStackedWidget();

	this->setObjectName("MainWidget");
	this->setWindowFlag(Qt::FramelessWindowHint);
	this->resize(880, 480);
	ui->headLab->installEventFilter(this);
	//窗口操作按钮图标
	ui->hideBtn->setIcon(QIcon(":/icon/Resource/Icon/hide.png"));
	ui->expandBtn->setIcon(QIcon(":/icon/Resource/Icon/expand.png"));
	ui->exitWidgetBtn->setIcon(QIcon(":/icon/Resource/Icon/x.png"));

	auto defaultPixmap = QPixmap(":/picture/Resource/Picture/user_defalut.png");
	ui->headLab->setPixmap(ImageUtils::roundedPixmap(defaultPixmap, QSize(60, 60)));
	//界面操作按钮图标
	ui->homeBtn->setCheckable(true);
	ui->roomBtn->setCheckable(true);
	ui->deviceBtn->setCheckable(true);
	ui->homeBtn->setChecked(true);
	//按钮组
	m_pageBtnGroup->setExclusive(true);
	m_pageBtnGroup->addButton(ui->homeBtn, static_cast<int>(PageBtn::HOMEPAGE));
	m_pageBtnGroup->addButton(ui->deviceBtn, static_cast<int>(PageBtn::DEVICEPAGE));
	m_pageBtnGroup->addButton(ui->roomBtn, static_cast<int>(PageBtn::ROOMPAGE));
	ui->homeBtn->setFixedSize(30, 30);
	ui->deviceBtn->setFixedSize(30, 30);
	ui->roomBtn->setFixedSize(30, 30);

	//窗口显示操作
	connect(ui->hideBtn, &QPushButton::clicked, this, [=]
		{
			emit hideWidget();
		});
	connect(ui->expandBtn, &QPushButton::clicked, this, [=]
		{
			emit expandWidget();
		});
	connect(ui->exitWidgetBtn, &QPushButton::clicked, this, [=]
		{
			emit exitWidget();
		});
	//账号退出
	connect(ui->exitBtn, &QPushButton::clicked, this, &MainWidget::onExitAccount);

	//界面切换
	connect(m_pageBtnGroup, &QButtonGroup::idClicked, this, &MainWidget::onSwitchWidget);

	//数据加载
	connect(LoginUserManager::instance(), &LoginUserManager::loginUserLoadSuccess, this, &MainWidget::onLoadData);
}

//初始化堆栈窗口
void MainWidget::initStackedWidget()
{
	ui->stackedWidget->addWidget(m_hoomPage);
	ui->stackedWidget->addWidget(m_devicePage);
	ui->stackedWidget->addWidget(m_roomPage);
	ui->stackedWidget->setCurrentWidget(m_hoomPage);
}

//数据加载
void MainWidget::onLoadData(const QJsonObject& obj)
{
	auto user_id = obj["user_id"].toString();
	auto user_name = obj["user_name"].toString();
	ui->user_idLab->setText(user_id);
	ui->user_nameLab->setText(user_name);
	m_user_id = user_id;

	ImageUtils::getUserAvatar(user_id, [this](QPixmap pixmap)
		{
			ui->headLab->setPixmap(ImageUtils::roundedPixmap(pixmap, QSize(60, 60)));
		});
}

//更改头像
void MainWidget::changeAvatar()
{
	//本地保存
	ImageUtils::saveAvatarToLocal(m_newAvatarPath, m_user_id);
	//通知服务端
	QtConcurrent::run([=]()
		{
			auto pixmap = QPixmap(m_newAvatarPath);
			QByteArray byteArray;
			QBuffer buffer(&byteArray);
			buffer.open(QIODevice::WriteOnly);
			if (!pixmap.save(&buffer, "PNG"))
			{
				qDebug() << "Failed to convert avatar to PNG format.";
				return;
			}

			QVariantMap params;
			params["user_id"] = m_user_id;
			params["size"] = byteArray.size();

			auto packet = PacketCreate::binaryPacket("updateUserAvatar", params, byteArray);
			QByteArray userData;
			PacketCreate::addPacket(userData, packet);
			auto allData = PacketCreate::allBinaryPacket(userData);

			QMap<QString, QString>headers;
			headers.insert("Content-Type", "application/octet-stream");

			// 发到主线程发信号
			QMetaObject::invokeMethod(NetWorkServiceLocator::instance(), [=]()
				{
					NetWorkServiceLocator::instance()->sendHttpPostRequest("updateUserAvatar", allData, headers);
				});
		});
}

//账号退出
void MainWidget::onExitAccount()
{
	NetWorkServiceLocator::instance()->disConnect();
	emit quitSuccess();
}

//界面切换
void MainWidget::onSwitchWidget(int widget_id)
{
	switch (widget_id)
	{
	case PageBtn::HOMEPAGE:
		ui->stackedWidget->setCurrentWidget(m_hoomPage);
		break;
	case PageBtn::DEVICEPAGE:
		ui->stackedWidget->setCurrentWidget(m_devicePage);
		break;
	case PageBtn::ROOMPAGE:
		ui->stackedWidget->setCurrentWidget(m_roomPage);

		break;
	default:
		break;
	}
}

//事件过滤
bool MainWidget::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == ui->headLab && event->type() == QEvent::MouseButtonPress)
	{
		m_newAvatarPath = QFileDialog::getOpenFileName(this, "选择头像", "",
			"Images(*.jpg *.png *.jpeg *.bnp)");
		if (!m_newAvatarPath.isEmpty())
		{
			m_avatarIsChanged = false;
			//头像是否更改
			if (m_newAvatarPath == m_oldAvatarPath)
			{
				return false;
			}
			QPixmap avatar(m_newAvatarPath);
			if (avatar.isNull())
			{
				// 头像加载失败，给出提示
				QMessageBox::warning(this, "错误", "无法加载图片，选择的文件不是有效的头像图片。");
				return false;  // 如果头像无效，返回 false
			}
			ui->headLab->setPixmap(ImageUtils::roundedPixmap(avatar, QSize(60, 60)));
			m_oldAvatarPath = m_newAvatarPath;
			m_avatarIsChanged = true;

			this->changeAvatar();
		}
	}
	return false;
}




