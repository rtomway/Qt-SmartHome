#include "HoomPage.h"
#include "ui_HoomPage.h"
#include <QDateTime>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

#include "../Client-ServiceLocator/NetWorkServiceLocator.h"
#include "EventBus.h"
#include "PacketCreate.h"



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
	connect(EventBus::instance(), &EventBus::allLightControl, this, &HoomPage::onAllLightStateChanged);
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

	m_lightContralCard->setSwitchState(true);
	m_lightContralCard->setSwitchState(false);

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
	auto topic = "smartHome/cmd";
	MqttJsonConfig allLightCfg;
	allLightCfg.product = "light";
	allLightCfg.device = "All_light";
	allLightCfg.property = "state";
	allLightCfg.value = state ? "on" : "off";
	QJsonObject lightObj = PacketCreate::mqttJsonConfig(allLightCfg);

	NetWorkServiceLocator::instance()->publishCmd(topic, lightObj);

	emit EventBus::instance()->allLightControl(state);
}

//通各个开关总体状态同步
void HoomPage::onAllLightStateChanged(bool state)
{
	m_lightContralCard->setSwitchState(state);
}


void HoomPage::addLightCard()
{
	QPixmap onPixmap(":/picture/Resource/Picture/allLightOn.png");
	QPixmap offPixmap(":/picture/Resource/Picture/allLightOff.png");
	m_lightContralCard = new DeviceContralCard("所有灯光", onPixmap, offPixmap, this);
}

