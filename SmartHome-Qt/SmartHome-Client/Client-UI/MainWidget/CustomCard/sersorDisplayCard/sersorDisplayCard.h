#ifndef SERSORDISPLAYCARD_H_
#define SERSORDISPLAYCARD_H_

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QPixmap>

class SersorDisplayCard :public QWidget
{
	Q_OBJECT
public:
	SersorDisplayCard(QString sersorName, QPixmap sersorPixmap, QWidget* parent = nullptr);
	~SersorDisplayCard();
	void setSersorValue(const QString& value);
private:
	void init();
	void initUi();
	void updateSersorValue(const QString& value);
private:
	QBoxLayout* m_layout;
	QLabel* m_sersorNameLab;
	QLabel* m_sersorPixLab;
	QLabel* m_sersorValueLab;
	QPixmap m_sersorPixmap;
};


#endif // !SERSORDISPLAYCARD_H_
