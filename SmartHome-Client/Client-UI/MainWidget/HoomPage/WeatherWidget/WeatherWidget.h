#ifndef WEATHERWIDGET_H_
#define WEATHERWIDGET_H_

#include <QWidget>
#include <QJsonObject>

namespace Ui { class WeatherWidget; }

class WeatherWidget :public QWidget
{
	Q_OBJECT
public:
	WeatherWidget(QWidget* parent = nullptr);
	~WeatherWidget();
private:
	void init();
	void initWeatherIconHash();
	void getWeatherData();
	void updateWeatherUi();
	void loadWeatherData();
private:
	Ui::WeatherWidget* ui;
private:
	int m_temperature;
	int m_humdity;
	QString m_weather;
	QString m_location;
	QHash<QString, QPixmap>m_weatherIconHash;
	QJsonObject m_weartherJson;
};

#endif // !WEATHERWIDGET_H_