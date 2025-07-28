#ifndef ROOMPAGE_H_
#define ROOMPAGE_H_

#include <QWidget>

namespace Ui { class RoomPage; }

class RoomPage :public QWidget
{
	Q_OBJECT
public:
	RoomPage(QWidget* parent = nullptr);
	~RoomPage();
private:
	void init();
private:
	Ui::RoomPage* ui;
};

#endif // !ROOMPAGE_H_