#include "DevicePage.h"
#include "ui_DevicePage.h"
#include <QJsonObject>
#include <QJsonDocument>

#include "../Client-ServiceLocator/NetWorkServiceLocator.h"
#include "EventBus.h"
#include "PacketCreate.h"

#define	LIGHT_COUNT 3
#define INDOORTEMP_ABNORMAL 38

DevicePage::DevicePage(QWidget* parent)
	:QWidget(parent)
	, ui(new Ui::DevicePage)
{
	ui->setupUi(this);
	init();
}

DevicePage::~DevicePage()
{

}

void DevicePage::init()
{
	//灯光
	QPixmap hallOnPixmap(":/picture/Resource/Picture/hallLighton.png");
	QPixmap hallOffPixmap(":/picture/Resource/Picture/hallLightoff.png");
	m_hallLight = new DeviceContralCard("大厅主灯", hallOnPixmap, hallOffPixmap, this);

	QPixmap bathOnPixmap(":/picture/Resource/Picture/bathLighton.png");
	QPixmap bathOffPixmap(":/picture/Resource/Picture/bathLightoff.png");
	m_bathroomLight = new DeviceContralCard("浴室灯光", bathOnPixmap, bathOffPixmap, this);

	QPixmap bedOnPixmap(":/picture/Resource/Picture/bedLighton.png");
	QPixmap bedOffPixmap(":/picture/Resource/Picture/bedLightoff.png");
	m_bedroomLight = new DeviceContralCard("卧室吊灯", bedOnPixmap, bedOffPixmap, this);

	ui->hallLightControl->layout()->addWidget(m_hallLight);
	ui->bedroomLightControl->layout()->addWidget(m_bedroomLight);
	ui->bathroomLightControl->layout()->addWidget(m_bathroomLight);

	//温湿度
	m_indoorTempDispaly = new SersorDisplayCard("室内温度", QPixmap(":/picture/Resource/Picture/indoorTemperature.png"), this);
	ui->indoorTemperatureWidget->layout()->addWidget(m_indoorTempDispaly);
	m_indoorLightDispaly = new SersorDisplayCard("室内光照", QPixmap(":/picture/Resource/Picture/indoorLight.png"), this);
	ui->indoorLightWidget->layout()->addWidget(m_indoorLightDispaly);

	//灯光控制
	connect(m_hallLight, &DeviceContralCard::SwitchState, this, &DevicePage::onHallLightChange);
	connect(m_bedroomLight, &DeviceContralCard::SwitchState, this, &DevicePage::onBedRoomLightChange);
	connect(m_bathroomLight, &DeviceContralCard::SwitchState, this, &DevicePage::onBathRoomLightChange);
	connect(EventBus::instance(), &EventBus::allLightControl, this, &DevicePage::onAllLightStateChanged);

	//温度、光照
	connect(EventBus::instance(), &EventBus::updateIndoorTemperature, this, &DevicePage::onUpdateIndoorTemp);
	connect(EventBus::instance(), &EventBus::updateIndoorHumidity, m_indoorLightDispaly, &SersorDisplayCard::updateSersorValue);
}

void DevicePage::onHallLightChange(bool state)
{
	sendLightCmd("hall_light", state);
}

void DevicePage::onBedRoomLightChange(bool state)
{
	sendLightCmd("bedroom_light", state);
}

void DevicePage::onBathRoomLightChange(bool state)
{
	sendLightCmd("bathroom_light", state);
}

//和总灯光控制同步
void DevicePage::onAllLightStateChanged(bool state)
{
	m_lightCount = state ? LIGHT_COUNT : 0;
	m_hallLight->setSwitchState(state);
	m_bathroomLight->setSwitchState(state);
	m_bedroomLight->setSwitchState(state);
}

//发送灯光指令
void DevicePage::sendLightCmd(const QString& device, bool state)
{
	auto topic = "smartHome/cmd";

	MqttJsonConfig lightConfig;
	lightConfig.product = "light";
	lightConfig.device = device;
	lightConfig.property = "state";
	lightConfig.value = state ? "on" : "off";
	auto lightObj = PacketCreate::mqttJsonConfig(lightConfig);

	NetWorkServiceLocator::instance()->publishCmd(topic, lightObj);

	//同步ui
	if (state)
		m_lightCount++;
	else
		m_lightCount--;

	if (m_lightCount == LIGHT_COUNT)
	{
		emit EventBus::instance()->allLightControl(true);
	}
	else if (m_lightCount == 0)
	{
		emit EventBus::instance()->allLightControl(false);
	}

}

//更新温度显示（限制阈值）
void DevicePage::onUpdateIndoorTemp(const QString& value)
{
	int tempValue = value.left(value.length() - 2).toFloat();
	if (tempValue > INDOORTEMP_ABNORMAL)
	{
		m_indoorTempDispaly->updateSersorName("室内温度异常");
		m_indoorTempDispaly->updateSersorPixmap(QPixmap(":/picture/Resource/Picture/indoorTempAbnormal.png"));
	}
	else
	{
		m_indoorTempDispaly->updateSersorName("室内温度");
		m_indoorTempDispaly->updateSersorPixmap(QPixmap(":/picture/Resource/Picture/indoorTemperature.png"));
	}
	m_indoorTempDispaly->updateSersorValue(value);
}

