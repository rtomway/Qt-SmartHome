#include "AngleRoundedWidget.h"
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

AngleRoundedWidget::AngleRoundedWidget(QWidget* parent)
{
	init();
}

void AngleRoundedWidget::init()
{
	this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
	this->setAttribute(Qt::WA_TranslucentBackground); 
}

void AngleRoundedWidget::paintEvent(QPaintEvent* ev)
{
	QStyleOption opt; 
	opt.rect=this->rect(); 
	QPainter p(this); 
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this); 
}
