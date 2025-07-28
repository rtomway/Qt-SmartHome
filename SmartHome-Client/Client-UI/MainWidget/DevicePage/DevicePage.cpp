#include "DevicePage.h"
#include "ui_DevicePage.h"

DevicePage::DevicePage(QWidget* parent)
	:QWidget(parent)
	, ui(new Ui::DevicePage)
{
	ui->setupUi(this);
	init();
}

DevicePage::~DevicePage()
{

}

void DevicePage::init()
{

}
