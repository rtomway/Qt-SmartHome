#include "HoomPage.h"
#include "ui_HoomPage.h"
#include <QDateTime>
#include <QFile>


HoomPage::HoomPage(QWidget* parent)
	:QWidget(parent)
	, ui(new Ui::HoomPage)
	, m_timer(new QTimer(this))
	, m_weatherWidget(new WeatherWidget(this))
{
	ui->setupUi(this);
	init();
	QFile file(":/stylesheet/Resource/StyleSheet/HoomPage.css");
	if (file.open(QIODevice::ReadOnly))
	{
		this->setStyleSheet(file.readAll());
	}
	
}

HoomPage::~HoomPage()
{
	
}

void HoomPage::init()
{
	this->setObjectName("HoomPage");
	this->setAutoFillBackground(true);
	m_weatherWidget->setStyleSheet("border:1px,solid,black;border-radius:6px");
	ui->weatherWidget->setContentsMargins(0, 0, 0, 0);
	ui->weatherWidget->setStyleSheet("border:1px,solid,black;border-radius:6px");
	auto time = QDateTime::currentDateTime();
	ui->current_timLab->setText(time.toString("hh:mm"));
	ui->dateLab->setText(time.toString("MM-dd"));
	QLocale local("zh_CN");
	ui->weekLab->setText(local.toString(time, "dddd"));

	m_timer->start(1000);
	connect(m_timer, &QTimer::timeout, this, &HoomPage::onUpdateCurrentTime);

	ui->weatherWidget->layout()->addWidget(m_weatherWidget);
	
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

