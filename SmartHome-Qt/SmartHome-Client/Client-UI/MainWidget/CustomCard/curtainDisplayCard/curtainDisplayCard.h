#ifndef CurtainDisplayCard_H_
#define CurtainDisplayCard_H_

#include <QWidget>
#include <QButtonGroup>
#include <QTimer>

namespace Ui { class CurtainDisplayCard; }


class CurtainDisplayCard :public QWidget
{
	Q_OBJECT
public:
	CurtainDisplayCard(QWidget* parent = nullptr);
	~CurtainDisplayCard();
private:
	void init();
	void initUi();
private:
	Ui::CurtainDisplayCard* ui;
private:
	QTimer* m_sliderTimer;
signals:
	void curtainValueChange(int percentage);

};


#endif // !CurtainDisplayCard_H_
