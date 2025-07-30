#include <QApplication>

#include "Client-UI/MainWindow.h"
#include "WebClient.h"
#include "HttpClient.h"
#include "ExternalHttpClient.h"
#include "NetWorkService.h"
#include "Client-ServiceLocator/NetWorkServiceLocator.h"

#include "WeatherWidget.h"

int main(int argc, char* argv[])
{

	QApplication a(argc, argv);

	//网络
	WebClient webClient;
	HttpClient httpClient;
	ExternalHttpClient externalHttpClient;
	NetWorkService service(&webClient, &httpClient, &externalHttpClient);
	NetWorkServiceLocator::setNetService(&service);

	//Ui
	MainWindow mainwindow;
	mainwindow.show();

	//WeatherWidget w;
	//w.show();

	return a.exec();
}

