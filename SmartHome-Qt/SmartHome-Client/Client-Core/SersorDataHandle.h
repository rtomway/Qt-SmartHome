#ifndef SERSORDATAHANDLE_H_
#define SERSORDATAHANDLE_H_

#include <QObject>
#include <QJsonObject>
#include <QByteArray>

class SersorDataHandle :public QObject
{
	Q_OBJECT
public:
	static void updateIndoorTemperature(const QJsonObject& paramsObj, const QByteArray& data = nullptr);
	static void updateIndoorHumidity(const QJsonObject& paramsObj, const QByteArray& data = nullptr);

};


#endif // !SERSORDATAHANDLE_H_
