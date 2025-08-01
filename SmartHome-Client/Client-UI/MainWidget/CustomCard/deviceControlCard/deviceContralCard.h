#ifndef DEVICECONTRALCARD_H_
#define DEVICECONTRALCARD_H_

#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include "SSwitchButton.h"

namespace Ui { class DeviceContralCard; }

class DeviceContralCard :public QWidget
{
	Q_OBJECT
public:
	DeviceContralCard(QString deviceName, QPixmap onDevicePixmap, QPixmap offDevicePixmap, QWidget* parent = nullptr);
	~DeviceContralCard();
private:
	void init();
	void initUi();
	void onSwitchState(bool state);
private:
	Ui::DeviceContralCard* ui;
	QString m_deviceName;
	QPixmap m_onDevicePixmap;
	QPixmap m_offDevicePixmap;
	SSwitchButton* m_switchBtn;
signals:
	void SwitchState(bool state);
};


#endif // !DEVICECONTRALCARD_H_
