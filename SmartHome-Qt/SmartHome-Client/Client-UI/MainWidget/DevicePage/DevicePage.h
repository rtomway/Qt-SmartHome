#ifndef DEVICEPAGE_H_
#define DEVICEPAGE_H_

#include <QWidget>
#include "CustomCard/deviceControlCard/deviceContralCard.h"

namespace Ui { class DevicePage; }

class DevicePage :public QWidget
{
	Q_OBJECT
public:
	DevicePage(QWidget* parent = nullptr);
	~DevicePage();
private:
	void init();
	void onHallLightChange(bool state);
	void onBedRoomLightChange(bool state);
	void onBathRoomLightChange(bool state);
	void onAllLightStateChanged(bool state);
	void sendLightCmd(const QString& device, bool state);
private:
	Ui::DevicePage* ui;
	DeviceContralCard* m_hallLight;
	DeviceContralCard* m_bedroomLight;
	DeviceContralCard* m_bathroomLight;
	int m_lightCount = 0;
};

#endif // !DEVICEPAGE_H_