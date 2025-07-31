#include <QApplication>

#include "Client-UI/MainWindow.h"
#include "HttpClient.h"
#include "ExternalHttpClient.h"
#include "NetWorkService.h"
#include "Client-ServiceLocator/NetWorkServiceLocator.h"


int main(int argc, char* argv[])
{

	QApplication a(argc, argv);

	//网络
	HttpClient httpClient;
	ExternalHttpClient externalHttpClient;
	NetWorkService service(&httpClient, &externalHttpClient);
	NetWorkServiceLocator::setNetService(&service);

	//Ui
	MainWindow mainwindow;
	mainwindow.show();


	return a.exec();
}

