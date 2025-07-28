#ifndef HOOMPAGE_H_
#define HOOMPAGE_H_

#include <QWidget>
#include <QTimer>

namespace Ui { class HoomPage; }

class HoomPage :public QWidget
{
	Q_OBJECT
public:
	HoomPage(QWidget* parent = nullptr);
	~HoomPage();
private:
	void init();
	void onUpdateCurrentTime();
private:
	Ui::HoomPage* ui;
	QTimer* m_timer;
};

#endif // !HOOMPAGE_H_