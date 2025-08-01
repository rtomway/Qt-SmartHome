#ifndef HOOMPAGE_H_
#define HOOMPAGE_H_

#include <QWidget>
#include <QTimer>

#include "CustomCard/WeatherWidget/WeatherWidget.h"
#include "CustomCard/deviceControlCard/deviceContralCard.h"

namespace Ui { class HoomPage; }

class HoomPage :public QWidget
{
	Q_OBJECT
public:
	HoomPage(QWidget* parent = nullptr);
	~HoomPage();
private:
	void init();
	void initUi();
	void addLightCard();
private:
	void onLightStateChanged(bool state);
	void onUpdateCurrentTime();
private:
	Ui::HoomPage* ui;
	QTimer* m_timer;
	WeatherWidget* m_weatherWidget;
	DeviceContralCard* m_lightContralCard;
};

#endif // !HOOMPAGE_H_