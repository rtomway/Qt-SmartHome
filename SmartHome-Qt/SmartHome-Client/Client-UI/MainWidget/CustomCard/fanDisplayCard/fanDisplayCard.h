#ifndef FANDISPLAYCARD_H_
#define FANDISPLAYCARD_H_

#include <QWidget>
#include <QButtonGroup>

namespace Ui { class FanDisplayCard; }

enum FAN_SPEED
{
	OFF,
	ON_LOW,
	ON_MIDUM,
	ON_HIGH
};

class FanDisplayCard :public QWidget
{
	Q_OBJECT
public:
	FanDisplayCard(QWidget* parent = nullptr);
	~FanDisplayCard();
	void setPixmap(QPixmap onPix, QPixmap offPix);
	void onChangeFanSpeed(int id);
private:
	void init();
	void initUi();
private:
	Ui::FanDisplayCard* ui;
	QPixmap m_fanOnPix;
	QPixmap m_fanOffPix;
	QButtonGroup* m_btnGroup;
signals:
	void changeFanSpeed(const QString& speedMode);
	
};


#endif // !FANDISPLAYCARD_H_
