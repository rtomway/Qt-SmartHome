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
	: QWidget(parent)
	, ui(new Ui::WeatherWidget)
	, m_updateWeatherTimer(new QTimer(this))
{
	ui->setupUi(this);
	init();
}

WeatherWidget::~WeatherWidget()
{

}

void WeatherWidget::init()
{
	initUI();
	initWeatherIconHash();
	fetchWeatherData();

	//半小时更新一次
	m_updateWeatherTimer->start(std::chrono::minutes(30));
	connect(m_updateWeatherTimer, &QTimer::timeout, this, &WeatherWidget::fetchWeatherData);
}

//初始化ui
void WeatherWidget::initUI()
{
	this->setObjectName("WeatherWidget");
	this->setAttribute(Qt::WA_StyledBackground, true);
	this->setStyleSheet(R"(  
		QWidget#WeatherWidget
		{
			border: 1px solid  rgb(77, 150, 205);
			border-radius: 10px;
			background-color: rgb(167, 214, 250);
		}
	)");
	ui->localTemperLab->setStyleSheet(R"( font-size: 26px;)");
	ui->temperBtn->setStyleSheet(R"( background-color: transparent;)");
	ui->humidityBtn->setStyleSheet(R"( background-color: transparent;)");

	ui->locationLab->setText("深圳市龙岗区");
	ui->temperBtn->setMinimumWidth(130);
	ui->humidityBtn->setMinimumWidth(130);
	ui->temperBtn->setIcon(QIcon(":/icon/Resource/Icon/temperature.png"));
	ui->humidityBtn->setIcon(QIcon(":/icon/Resource/Icon/humdity.png"));
}

//初始化weather对应icon
void WeatherWidget::initWeatherIconHash()
{
	m_weatherIconHash["晴"] = QPixmap(":/picture/Resource/Picture/sun.png");
	m_weatherIconHash["多云"] = QPixmap(":/picture/Resource/Picture/cloudy.png");
	m_weatherIconHash["小雨"] = QPixmap(":/picture/Resource/Picture/light_rain.png");
	m_weatherIconHash["中雨"] = QPixmap(":/picture/Resource/Picture/Moderate_rain.png");
	m_weatherIconHash["阵雨"] = QPixmap(":/picture/Resource/Picture/Moderate_rain.png");
	m_weatherIconHash["大雨"] = QPixmap(":/picture/Resource/Picture/heavy_rain.png");
	m_weatherIconHash["雷阵雨"] = QPixmap(":/picture/Resource/Picture/Thunder_storm.png");
	m_weatherIconHash["阴"] = QPixmap(":/picture/Resource/Picture/cloudy.png");
}

//获取天气数据
void WeatherWidget::fetchWeatherData()
{
	//当前地址
	QUrl url("https://pr3wryuh6r.re.qweatherapi.com/v7/weather/now");
	QUrlQuery query(url);
	query.addQueryItem("location", "101280606");
	query.addQueryItem("key", "76f0fb906675488bb67b86667788e7ba");

	//向和风天气发出请求
	NetWorkServiceLocator::instance()->sendExternalHttpGetRequest(url.toString(), query,
		[this](const QJsonObject& obj, const QByteArray& data)
		{
			if (obj["code"].toString().toInt() == 200)
			{
				m_weartherJson = obj["now"].toObject();
				loadWeatherData();
				updateWeatherUi();
			}
		});
}

//数据更新
void WeatherWidget::loadWeatherData()
{
	m_temperature = m_weartherJson["temp"].toString().toInt();
	m_humidity = m_weartherJson["humidity"].toString().toInt();
	m_weather = m_weartherJson["text"].toString();
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
	ui->temperBtn->setText("当天温度:" + QString(QString::number(m_temperature) + "%1C").arg(degreeSymbol));
	ui->humidityBtn->setText("相对湿度:" + QString(QString::number(m_humidity) + "%"));

}

