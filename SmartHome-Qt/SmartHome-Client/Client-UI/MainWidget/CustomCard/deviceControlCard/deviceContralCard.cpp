#include "deviceContralCard.h"
#include "ui_deviceContralCard.h"


DeviceContralCard::DeviceContralCard(QString deviceName, QPixmap onDevicePixmap, QPixmap offDevicePixmap, QWidget* parent)
	:QWidget(parent)
	, ui(new Ui::DeviceContralCard)
	, m_deviceName(deviceName)
	, m_onDevicePixmap(onDevicePixmap)
	, m_offDevicePixmap(offDevicePixmap)
	, m_switchBtn(new SSwitchButton(this))
{
	ui->setupUi(this);
	init();
}

DeviceContralCard::~DeviceContralCard()
{

}

void DeviceContralCard::setSwitchState(bool state)
{
	m_switchBtn->setToggle(state);
	if (state)
	{
		ui->switchLab->setText("ON");
		ui->switchLab->setStyleSheet("color:#409EFF;");
		ui->devicePixmapLab->setPixmap(m_onDevicePixmap);
	}
	else
	{
		ui->switchLab->setText("OFF");
		ui->switchLab->setStyleSheet("color:black;");
		ui->devicePixmapLab->setPixmap(m_offDevicePixmap);
	}
}

void DeviceContralCard::init()
{
	initUi();

	connect(m_switchBtn, &SSwitchButton::stateChanged, this, &DeviceContralCard::onSwitchState);
}

void DeviceContralCard::initUi()
{
	this->setFixedSize(200, 200);
	this->setObjectName("DeviceContralCard");
	this->setAttribute(Qt::WA_StyledBackground, true);
	this->setStyleSheet(R"(  
		QWidget#DeviceContralCard
		{
			border: 1px solid rgb(216, 238, 253);
			border-radius: 10px;
			background-color:  rgb(167, 214, 250);
		}
	)");

	m_onDevicePixmap = m_onDevicePixmap.scaled(80, 80);
	m_offDevicePixmap = m_offDevicePixmap.scaled(80, 80);

	ui->devicePixmapLab->setFixedSize(150, 150);
	ui->devicePixmapLab->setPixmap(m_offDevicePixmap);
	ui->devicenameLab->setText(m_deviceName);
	ui->devicenameLab->setStyleSheet("font-size:18px;");
	ui->switchLab->setText("OFF");
	ui->switchLab->setStyleSheet("color:black;");

	m_switchBtn->setFixedSize(53, 27);
	ui->switchBtnLayout->addWidget(m_switchBtn);

}

void DeviceContralCard::onSwitchState(bool state)
{
	emit SwitchState(state);
	if (state)
	{
		ui->switchLab->setText("ON");
		ui->switchLab->setStyleSheet("color:#409EFF;");
		ui->devicePixmapLab->setPixmap(m_onDevicePixmap);
	}
	else
	{
		ui->switchLab->setText("OFF");
		ui->switchLab->setStyleSheet("color:black;");
		ui->devicePixmapLab->setPixmap(m_offDevicePixmap);
	}
}
