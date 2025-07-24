#include "MainWidget.h"
#include "ui_MainWidget.h"
#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QBoxlayout>
#include <QLabel>
#include <QMouseEvent>
#include <memory>
#include <QPointer>
#include <QJsonDocument>
#include <QJsonObject>

#include "ImageUtil.h"
#include "SMaskWidget.h"


MainWidget::MainWidget(QWidget* parent)
	:AngleRoundedWidget(parent)
	, ui(new Ui::MainWidget)
{
	ui->setupUi(this);
	init();
	
	this->setStyleSheet(R"(QWidget#MainWidget{border-radius: 10px;})");
	QFile file(":/stylesheet/Resource/StyleSheet/MainWidget.css");
	if (file.open(QIODevice::ReadOnly))
	{
		setStyleSheet(file.readAll());
	}
	else {
		qDebug() << "样式表打开失败";
	}
	//编辑信息蒙层主窗口
	SMaskWidget::instance()->setMainWidget(this);
	setMouseTracking(true);  // 主窗口自身
}

MainWidget::~MainWidget()
{
	delete ui;
}

//界面初始化
void MainWidget::init()
{
	this->setObjectName("MainWidget");
	this->setWindowFlag(Qt::FramelessWindowHint);
	this->resize(1080, 680);

	//安装事件过滤器
	this->installEventFilter(this);
}




