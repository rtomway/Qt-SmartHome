#include "RoomPage.h"
#include "ui_RoomPage.h"

RoomPage::RoomPage(QWidget* parent)
	:QWidget(parent)
	, ui(new Ui::RoomPage)
{
	ui->setupUi(this);
	init();
}

RoomPage::~RoomPage()
{

}

void RoomPage::init()
{

}
