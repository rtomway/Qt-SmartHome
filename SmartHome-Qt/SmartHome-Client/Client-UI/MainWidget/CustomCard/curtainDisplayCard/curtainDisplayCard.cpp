#include "CurtainDisplayCard.h"
#include "ui_CurtainDisplayCard.h"

CurtainDisplayCard::CurtainDisplayCard(QWidget* parent)
	:QWidget(parent)
	, ui(new Ui::CurtainDisplayCard)
	, m_sliderTimer(new QTimer(this))
{
	ui->setupUi(this);
	init();
}

CurtainDisplayCard::~CurtainDisplayCard()
{

}


void CurtainDisplayCard::init()
{
	initUi();
	m_sliderTimer->setSingleShot(true);
	connect(ui->curtainSlider, &QSlider::valueChanged, this, [=](int value)
		{
			ui->percentLab->setText("窗帘:"+QString::number(value) + "%");
			m_sliderTimer->stop();
			m_sliderTimer->start(300);  
		});

	connect(m_sliderTimer,&QTimer::timeout, this, [=]() 
		{
			int value = ui->curtainSlider->value();
			emit curtainValueChange(value);
		});

}

void CurtainDisplayCard::initUi()
{
	ui->pixmapLab->setPixmap(QPixmap(":/picture/Resource/Picture/curtain.png").scaled(180, 180));

	this->setFixedSize(200, 200);
	this->setObjectName("CurtainDisplayCard");
	this->setAttribute(Qt::WA_StyledBackground, true);
	this->setStyleSheet(R"(
	QWidget#CurtainDisplayCard
		{
			border: 1px solid rgb(216, 238, 253);
			border-radius: 10px;
			background-color:  rgb(167, 214, 250);
		}
	QLabel#percentLab
	{
		font-size:20px;
		color:#0094ff;
	}
	)");

	ui->percentLab->setText("窗帘:0%");
}
