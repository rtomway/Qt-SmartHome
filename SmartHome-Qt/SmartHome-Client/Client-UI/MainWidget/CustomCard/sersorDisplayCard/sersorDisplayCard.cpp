#include "sersorDisplayCard.h"
#include "sersorDisplayCard.h"


SersorDisplayCard::SersorDisplayCard(QString sersorName, QPixmap sersorPixmap, QWidget* parent)
	:QWidget(parent)
	, m_layout(new QVBoxLayout(this))
	, m_sersorNameLab(new QLabel(sersorName, this))
	, m_sersorPixLab(new QLabel(this))
	, m_sersorValueLab(new QLabel(this))
	, m_sersorPixmap(sersorPixmap)
{
	init();
}

SersorDisplayCard::~SersorDisplayCard()
{

}

void SersorDisplayCard::setSersorValue(const QString& value)
{
	m_sersorValueLab->setText(value);
}

void SersorDisplayCard::init()
{

	initUi();

}

void SersorDisplayCard::initUi()
{
	this->setFixedSize(200, 200);
	this->setObjectName("SersorDisplayCard");
	this->setAttribute(Qt::WA_StyledBackground, true);
	this->setStyleSheet(R"(  
		QWidget#SersorDisplayCard
		{
			border: 1px solid rgb(216, 238, 253);
			border-radius: 10px;
			background-color:  rgb(167, 214, 250);
		}
	)");

	m_layout->addWidget(m_sersorNameLab);
	m_layout->addWidget(m_sersorPixLab);
	m_layout->addWidget(m_sersorValueLab);

	m_sersorNameLab->setAlignment(Qt::AlignCenter);
	m_sersorPixLab->setAlignment(Qt::AlignCenter);
	m_sersorValueLab->setAlignment(Qt::AlignCenter);

	m_sersorPixmap = m_sersorPixmap.scaled(80, 80);
	m_sersorPixLab->setPixmap(m_sersorPixmap);
	m_sersorValueLab->setText(QString::number(37) + "C");

	m_layout->setContentsMargins(6, 12, 6, 6);
	m_layout->setStretch(0, 1);
	m_layout->setStretch(1, 1);
	m_layout->setStretch(2, 3);

	m_sersorValueLab->setStyleSheet("font-size:18px;");

}

//更新传感器数据
void SersorDisplayCard::updateSersorValue(const QString& value)
{
	m_sersorValueLab->setText(value);
}
