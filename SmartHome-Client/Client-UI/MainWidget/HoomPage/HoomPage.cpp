#include "HoomPage.h"
#include "ui_HoomPage.h"
#include <QDateTime>
#include <QFile>


HoomPage::HoomPage(QWidget* parent)
	:QWidget(parent)
	, ui(new Ui::HoomPage)
	, m_timer(new QTimer(this))
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


	auto time = QDateTime::currentDateTime();
	ui->current_timLab->setText(time.toString("hh:mm"));
	ui->dateLab->setText(time.toString("MM-dd"));
	QLocale local("zh_CN");
	ui->weekLab->setText(local.toString(time, "dddd"));

	m_timer->start(1000);
	connect(m_timer, &QTimer::timeout, this, &HoomPage::onUpdateCurrentTime);

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
