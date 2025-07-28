#include "WeatherWidget.h"
#include "ui_WeatherWidget.h"
#include <QUrl>
#include <QUrlQuery>
#include <QJsonArray>
#include <QFile>
#include <QChar>

#include "../Client-ServiceLocator/NetWorkServiceLocator.h"
#include "ImageUtil.h"

WeatherWidget::WeatherWidget(QWidget* parent)
	:QWidget(parent)
	,ui(new Ui::WeatherWidget)
{
	ui->setupUi(this);
	init();
	QFile file(":/stylesheet/Resource/StyleSheet/WeatherWidget.css");
	if (file.open(QIODevice::ReadOnly))
	{
		this->setStyleSheet(file.readAll());
	}
}

WeatherWidget::~WeatherWidget()
{

}

void WeatherWidget::init()
{
	this->setStyleSheet(" background-color: red;border:1px,solid,black;");
	this->setAutoFillBackground(true);
	this->setObjectName("WeatherWidget");
	this->setFixedSize(300, 150);
	initWeatherIconHash();
	getWeatherData();
	ui->locationLab->setText("深圳市龙岗区");
	ui->temperBtn->setMinimumWidth(130);
	ui->humdityBtn->setMinimumWidth(130);
	ui->temperBtn->setIcon(QIcon(":/icon/Resource/Icon/temperature.png"));
	ui->humdityBtn->setIcon(QIcon(":/icon/Resource/Icon/humdity.png"));
}

//初始化weather对应icon
void WeatherWidget::initWeatherIconHash()
{
	m_weatherIconHash["晴天"] = QPixmap(":/picture/Resource/Picture/sun.png");
	m_weatherIconHash["多云"] = QPixmap(":/picture/Resource/Picture/cloudy.png");
	m_weatherIconHash["小雨"] = QPixmap(":/picture/Resource/Picture/light_rain.png");
	m_weatherIconHash["中雨"] = QPixmap(":/picture/Resource/Picture/Moderate_rain.png");
	m_weatherIconHash["阵雨"] = QPixmap(":/picture/Resource/Picture/Moderate_rain.png");
	m_weatherIconHash["大雨"] = QPixmap(":/picture/Resource/Picture/heavy_rain.png");
	m_weatherIconHash["雷阵雨"] = QPixmap(":/picture/Resource/Picture/Thunder_storm.png");
}

//获取天气数据
void WeatherWidget::getWeatherData()
{
	//当前地址
	QUrl url("https://pr3wryuh6r.re.qweatherapi.com/v7/weather/3d");
	QUrlQuery query(url);
	query.addQueryItem("location", "101280606");
	query.addQueryItem("key", "76f0fb906675488bb67b86667788e7ba");

	//向和风天气发出请求
	NetWorkServiceLocator::instance()->sendExternalHttpGetRequest(url.toString(), query, QMap<QString, QString>(),
		[this](const QJsonObject& obj, const QByteArray& data)
		{
			if (obj["code"].toString().toInt() == 200)
			{
				QJsonArray weatherArray = obj["daily"].toArray();
				m_weartherJson= weatherArray.at(0).toObject();
				loadWeatherData();
				updateWeatherUi();
			}
		});
}

//数据更新
void WeatherWidget::loadWeatherData()
{
	m_temperature = m_weartherJson["tempMin"].toString().toInt() / 2 + m_weartherJson["tempMax"].toString().toInt() / 2;
	m_humdity = m_weartherJson["humidity"].toString().toInt();
	m_weather = m_weartherJson["textDay"].toString();
}


//UI更新
void WeatherWidget::updateWeatherUi()
{
	ui->weatherLab->setText(m_weather);
	if (m_weatherIconHash.contains(m_weather))
	{
		auto weatherIcon = m_weatherIconHash.value(m_weather);
		ui->weatherIconLab->setPixmap(ImageUtils::roundedPixmap(weatherIcon, QSize(80, 80)));
	}
	else
	{
		auto weatherPix = QPixmap(":/picture/Resource/Picture/cloudy.png");
		ui->weatherIconLab->setPixmap(ImageUtils::roundedPixmap(weatherPix, QSize(80, 80)));
	}
	QChar degreeSymbol(0x00B0);
	ui->localTemperLab->setText(QString(QString::number(m_temperature) + "%1C").arg(degreeSymbol));
	ui->temperBtn->setText("当前温度:" + QString(QString::number(m_temperature) + "%1C").arg(degreeSymbol));
	ui->humdityBtn->setText("相对湿度:" + QString(QString::number(m_humdity)+"%"));
	
}

