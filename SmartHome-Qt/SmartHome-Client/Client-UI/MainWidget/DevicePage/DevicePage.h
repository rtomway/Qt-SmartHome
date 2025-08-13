﻿#ifndef DEVICEPAGE_H_
#define DEVICEPAGE_H_

#include <QWidget>
#include "CustomCard/deviceControlCard/deviceContralCard.h"
#include "CustomCard/sersorDisplayCard/sersorDisplayCard.h"

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
	void onUpdateIndoorTemp(const QString& value);
private:
	Ui::DevicePage* ui;
	DeviceContralCard* m_hallLight;
	DeviceContralCard* m_bedroomLight;
	DeviceContralCard* m_bathroomLight;
	SersorDisplayCard* m_indoorTempDispaly;
	SersorDisplayCard* m_indoorLightDispaly;
	int m_lightCount = 0;
};

#endif // !DEVICEPAGE_H_