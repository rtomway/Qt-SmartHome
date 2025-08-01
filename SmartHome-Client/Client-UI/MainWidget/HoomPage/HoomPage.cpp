#include "HoomPage.h"
#include "ui_HoomPage.h"
#include <QDateTime>
#include <QFile>



HoomPage::HoomPage(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::HoomPage)
	, m_timer(new QTimer(this))
	, m_weatherWidget(new WeatherWidget(this))
{
	ui->setupUi(this);
	init();
}

HoomPage::~HoomPage()
{

}

void HoomPage::init()
{
	initUi();

	//更新时钟(每分钟)
	m_timer->start(std::chrono::minutes(1));
	connect(m_timer, &QTimer::timeout, this, &HoomPage::onUpdateCurrentTime);

	//灯光控制
	connect(m_lightContralCard, &DeviceContralCard::SwitchState, this, &HoomPage::onLightStateChanged);
}

void HoomPage::initUi()
{
	//当前窗口设置
	this->setObjectName("HoomPage");
	this->setAttribute(Qt::WA_StyledBackground, true);
	QFile file(":/stylesheet/Resource/StyleSheet/HoomPage.css");
	if (file.open(QIODevice::ReadOnly))
	{
		this->setStyleSheet(file.readAll());
	}
	else {
		qDebug() << "样式表打开失败";
	}
	//子控件
	auto time = QDateTime::currentDateTime();
	ui->current_timLab->setText(time.toString("hh:mm"));
	ui->dateLab->setText(time.toString("MM-dd"));
	QLocale local("zh_CN");
	ui->weekLab->setText(local.toString(time, "dddd"));

	ui->weatherWidget->layout()->addWidget(m_weatherWidget);
	ui->weatherWidget->setFixedSize(300, 150);

	addLightCard();
	ui->lightCardWidget->layout()->addWidget(m_lightContralCard);

}

//更新当前时间
void HoomPage::onUpdateCurrentTime()
{
	auto time = QDateTime::currentDateTime();
	ui->current_timLab->setText(time.toString("hh:mm"));
	ui->dateLab->setText(time.toString("MM-dd"));
	QLocale local("zh_CN");
	ui->weekLab->setText(local.toString(time, "dddd"));
}

//所有灯光一键控制
void HoomPage::onLightStateChanged(bool state)
{

}


void HoomPage::addLightCard()
{
	QPixmap onPixmap(":/picture/Resource/Picture/allLightOn.png");
	QPixmap offPixmap(":/picture/Resource/Picture/allLightOff.png");
	m_lightContralCard = new DeviceContralCard("所有灯光", onPixmap, offPixmap, this);
}

