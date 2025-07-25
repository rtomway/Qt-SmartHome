#ifndef MAINWIDGET_H_
#define MAINWIDGET_H_

#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QMenu>

#include "AngleRoundedWidget.h"

namespace Ui { class MainWidget; }

enum PageBtn
{
	HOMEPAGE,
	DEVICEPAGE,
	ROOMPAGE
};

class MainWidget :public AngleRoundedWidget
{
	Q_OBJECT
public:
	MainWidget(QWidget* parent = nullptr);
	~MainWidget();
private:
	void init();
	void onLoadData(const QJsonObject&obj);
	void changeAvatar();
private:
	Ui::MainWidget* ui;
	QString m_user_id;
	QButtonGroup* m_pageBtnGroup{};
	QString m_newAvatarPath;
	QString m_oldAvatarPath;
	bool m_avatarIsChanged = false;
protected:
	bool eventFilter(QObject* watched, QEvent* event)override;
signals:
	void quitSuccess();
	void hideWidget();
	void expandWidget();
	void exitWidget();
};

#endif // !MAINWIDGET_H_
