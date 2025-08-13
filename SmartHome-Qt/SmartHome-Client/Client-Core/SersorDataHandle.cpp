#include "SersorDataHandle.h"
#include "EventBus.h"

void SersorDataHandle::updateIndoorTemperature(const QJsonObject& paramsObj, const QByteArray& data)
{
	QChar degreeSymbol(0x00B0);
	auto value = paramsObj["value"].toString() + QString("%1C").arg(degreeSymbol);
	EventBus::instance()->emit updateIndoorTemperature(value);
}

void SersorDataHandle::updateIndoorHumidity(const QJsonObject& paramsObj, const QByteArray& data)
{
	auto value = paramsObj["value"].toString() + QString("lux");
	EventBus::instance()->emit updateIndoorHumidity(value);
}
