#include <QApplication>

#include "Client-UI/MainWindow.h"
#include "HttpClient.h"
#include "ExternalHttpClient.h"
#include "MqttClient.h"
#include "NetWorkService.h"
#include "Client-ServiceLocator/NetWorkServiceLocator.h"

#include <QMqttClient>


int main(int argc, char* argv[])
{

	QApplication a(argc, argv);


	//网络
	MqttClient mqttClient;
	HttpClient httpClient;
	ExternalHttpClient externalHttpClient;

	NetWorkService service(&mqttClient, &httpClient, &externalHttpClient);
	NetWorkServiceLocator::setNetService(&service);

	////Ui
	MainWindow mainwindow;
	mainwindow.show();


	return a.exec();
}

