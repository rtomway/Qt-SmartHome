#include "fanDisplayCard.h"
#include "ui_fanDisplayCard.h"

FanDisplayCard::FanDisplayCard(QWidget* parent)
	:QWidget(parent)
	, ui(new Ui::FanDisplayCard)
	, m_btnGroup(new QButtonGroup(this))
{
	ui->setupUi(this);
	init();
}

FanDisplayCard::~FanDisplayCard()
{

}


void FanDisplayCard::init()
{
	initUi();


	connect(m_btnGroup, &QButtonGroup::idClicked, this, &FanDisplayCard::onChangeFanSpeed);
}

void FanDisplayCard::initUi()
{
	m_btnGroup->addButton(ui->closeBtn,0);
	m_btnGroup->addButton(ui->oneBtn,1);
	m_btnGroup->addButton(ui->twoBtn,2);
	m_btnGroup->addButton(ui->threeBtn,3);
	m_btnGroup->setExclusive(true);

	ui->closeBtn->setCheckable(true);
	ui->oneBtn->setCheckable(true);
	ui->twoBtn->setCheckable(true);
	ui->threeBtn->setCheckable(true);

	ui->closeBtn->setChecked(true);

	this->setFixedSize(200, 200);
	this->setObjectName("FanDisplayCard");
	this->setAttribute(Qt::WA_StyledBackground, true);
	this->setStyleSheet(R"(
	QWidget#FanDisplayCard
		{
			border: 1px solid rgb(216, 238, 253);
			border-radius: 10px;
			background-color:  rgb(167, 214, 250);
		}
		QPushButton{background-color:transparent;border-radius:5px;height:30px;width:50px;}
		QPushButton:hover{background-color:rgba(255,255,255,0.5);}
		QPushButton:checked{background-color:#298DFF;}
	)");

	

	m_fanOnPix = QPixmap(":/picture/Resource/Picture/fan_on.png").scaled(150,150);
	m_fanOffPix = QPixmap(":/picture/Resource/Picture/fan_off.png").scaled(150, 150);

	ui->closeBtn->setText("关闭");
	ui->oneBtn->setText("一档");
	ui->twoBtn->setText("二档");
	ui->threeBtn->setText("三档");

	ui->fanPixLab->setPixmap(m_fanOffPix);
}

//设置图片
void FanDisplayCard::setPixmap(QPixmap onPix, QPixmap offPix)
{
	m_fanOnPix = onPix;
	m_fanOffPix = offPix;
}

//改变风速
void FanDisplayCard::onChangeFanSpeed(int id)
{
	if (id == 0)
	{
		ui->fanPixLab->setPixmap(m_fanOffPix);
	}
	else
	{
		ui->fanPixLab->setPixmap(m_fanOnPix);
	}
	switch (id)
	{
	case 0:
		emit changeFanSpeed("off");
		break;
	case 1:
		emit changeFanSpeed("low");
		break;
	case 2:
		emit changeFanSpeed("medium");
		break;
	case 3:
		emit changeFanSpeed("high");
		break;
	default:
		break;
	}
}