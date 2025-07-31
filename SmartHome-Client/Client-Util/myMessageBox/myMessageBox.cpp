#include "myMessageBox.h"
#include <QLabel>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTimer>
#include <QPointer>

MyMessageBox::MyMessageBox(QWidget* parent, const QString& title, const QString& text, int time)
	:QMessageBox(parent)
	, m_title(title)
	, m_text(text)
	, m_time(std::chrono::milliseconds(time))
{
	init();
}

MyMessageBox::~MyMessageBox()
{

}



void MyMessageBox::init()
{
	m_currentPath = QFileInfo(__FILE__).path();
	QFile file(m_currentPath + "/myMessageBox.css");
	if (file.open(QIODevice::ReadOnly))
	{
		this->setStyleSheet(file.readAll());
	}
	else
	{
		qDebug() << "file css failed";
	}
	
	this->setWindowIcon(QIcon(m_currentPath + "/messageBox.png"));
	this->setWindowTitle(m_title);
	this->setText(m_text);

	if (m_time != std::chrono::milliseconds(0))
	{
		this->setStandardButtons(QMessageBox::NoButton);
		QTimer::singleShot(m_time,[this]
			{
				this->accept();
			});
	}
	
	this->exec();
}

void MyMessageBox::showEvent(QShowEvent* event)
{
	QLabel* atextField = findChild<QLabel*>("qt_msgbox_label");
	if (atextField != NULL)
	{
		atextField->setMinimumSize(250, 70);
	}

	QMessageBox::showEvent(event);
}
