#ifndef MYMESSAGEBOX
#define MYMESSAGEBOX

#include <QWidget>
#include <QMessageBox>

class MyMessageBox :public QMessageBox
{
	Q_OBJECT
public:
	MyMessageBox(QWidget* parent,const QString& title, const QString& text, int time=0);
	~MyMessageBox();
private:
	void init();
protected:
	void showEvent(QShowEvent* event);
private:
	QString m_title;
	QString m_text;
	QString m_currentPath;
	std::chrono::milliseconds m_time;
};

#endif // !MYMESSAGEBOX
