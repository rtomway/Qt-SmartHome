#ifndef DEVICEPAGE_H_
#define DEVICEPAGE_H_

#include <QWidget>

namespace Ui { class DevicePage; }

class DevicePage :public QWidget
{
	Q_OBJECT
public:
	DevicePage(QWidget* parent = nullptr);
	~DevicePage();
private:
	void init();
private:
	Ui::DevicePage* ui;
};

#endif // !DEVICEPAGE_H_